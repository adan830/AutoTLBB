#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "player_control_tabs/GeneralTab.hpp"
#include "player_control_tabs/ItemTab.hpp"
#include "player_control_tabs/SkillTab.hpp"

#include <QMessageBox>
#include <QFile>
#include <QDebug>

#include <cstring>

#include <Windows.h>
#include <tchar.h>
#include <psapi.h>

#include "Constants.hpp"
#include "games_window/GameWindowInfo.hpp"
#include "games_window/Player.hpp"
#include "player_control_tabs/TabAbstract.hpp"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->setMinimumSize(this->size());
  ui->gameListTableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

  {
    QFile styleSheet(":/qdarkstyle/style.qss");
    styleSheet.open(QFile::ReadOnly);
    this->setStyleSheet(styleSheet.readAll());
    styleSheet.close();
  }
  {
//    QFile styleSheet(":/qdarkstyle/app.qss");
//    styleSheet.open(QFile::ReadOnly);
//    styleSheet.close();
  }

  m_autoControlWidgets["General"] = GeneralTab::getInstance();
  m_autoControlWidgets["Item"] = ItemTab::getInstance();
  m_autoControlWidgets["Skill"] = SkillTab::getInstance();

  this->init();
}

MainWindow::~MainWindow()
{
  delete ui;

  for (const auto& autoControlWidget : m_autoControlWidgets)
  {
    delete autoControlWidget.second;
  }

  for (const auto& gameWindowInfo : m_gamesWindowInfo)
  {
    delete gameWindowInfo;
  }
}

bool MainWindow::init()
{
  this->initAutoControlWidgets();

  ui->autoControlTabWidget->setEnabled(false);

  if (!this->initGamesProcess())
  {
    return false;
  }

  if (!this->initGamesPlayerList())
  {
    return false;
  }

  for (int i = 0; i < ui->autoControlTabWidget->count(); i++)
  {
    auto widget = ui->autoControlTabWidget->widget(i);
    if (auto castWidget = dynamic_cast<TabAbstract*>(widget))
    {
      castWidget->setCurrentGameWindowInfo(*m_gamesWindowInfo.cbegin());
    }
  }

  ui->autoControlTabWidget->setEnabled(true);

  return true;
}

bool MainWindow::initAutoControlWidgets()
{
  for (const auto& widget : m_autoControlWidgets)
  {
    bool tabExists = false;
    for (int i = 0; i < ui->autoControlTabWidget->count(); i++)
    {
      if (ui->autoControlTabWidget->find(i) == widget.second)
      {
        tabExists = true;
      }
    }
    if (tabExists)
    {
      continue;
    }
    ui->autoControlTabWidget->addTab(widget.second, widget.first);
  }

  return true;
}

bool MainWindow::initGamesPlayerList()
{
  ui->gameListTableWidget->setRowCount(m_gamesWindowInfo.size());

  //  return false;
  for (std::size_t i = 0; i < m_gamesWindowInfo.size(); i++)
  {
    auto gameWindowInfo = m_gamesWindowInfo.at(i);

    auto player = gameWindowInfo->getPlayer();

    gameWindowInfo->setWindowName(player->getName());

    auto no = new QTableWidgetItem(QString::number(i + 1));
    ui->gameListTableWidget->setItem(i, 0, no);

    auto username = new QTableWidgetItem(player->getName());
    ui->gameListTableWidget->setItem(i, 1, username);

    auto hp = new QTableWidgetItem(QString::number(player->getHpPercent()));
    ui->gameListTableWidget->setItem(i, 2, hp);

    auto mp = new QTableWidgetItem(QString::number(player->getMpPercent()));
    ui->gameListTableWidget->setItem(i, 3, mp);

    auto pet_hp = new QTableWidgetItem("1");
    ui->gameListTableWidget->setItem(i, 4, pet_hp);

    auto mode = new QTableWidgetItem("1");
    ui->gameListTableWidget->setItem(i, 5, mode);

    ui->gameListTableWidget->setSizeAdjustPolicy(QTableWidget::QAbstractItemView::AdjustToContents);
  }

  return true;
}

static BOOL CALLBACK EnumWindowsProcCallback(HWND hwnd, LPARAM lParam)
{
  char className[MAX_PATH];
  ::GetClassNameA(hwnd, className, sizeof(className));

  auto gamesWindowInfo = reinterpret_cast<ListGameWindowInfo*>(lParam);

  DWORD processId;
  HANDLE handle = nullptr;
  for (const auto& name : constants::gameClassNames)
  {
    if (std::strcmp(name, className) != 0)
    {
      continue;
    }

    ::GetWindowThreadProcessId(hwnd, &processId);
    bool isExistsProcess = false;
    for (std::size_t i = 0; i < gamesWindowInfo->size(); i++)
    {
      if (gamesWindowInfo->at(i)->getProcessId() == processId)
      {
        isExistsProcess = true;
        break;
      }
    }
    if (isExistsProcess)
    {
      continue;
    }

    handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (handle == nullptr)
    {
      qDebug() << "Can not open game";
    }

    auto gameWindowInfo = new GameWindowInfo();
    gameWindowInfo->setHwnd(hwnd);
    gameWindowInfo->setProcessId(processId);
    gameWindowInfo->setHandle(handle);

    qDebug() << *gameWindowInfo;

    gamesWindowInfo->push_back(gameWindowInfo);
  }

  return TRUE;
}

bool MainWindow::initGamesProcess()
{
  ::EnumWindows(&EnumWindowsProcCallback, reinterpret_cast<LPARAM>(&m_gamesWindowInfo));

  for (const auto& gameWindowInfo : m_gamesWindowInfo)
  {
    if (gameWindowInfo != nullptr)
    {
      gameWindowInfo->getPlayer()->sendChatMsg("ssss");
      qDebug() << "Sent";
    }
  }

  return !m_gamesWindowInfo.empty();
}

void MainWindow::on_actionReload_Player_List_triggered()
{
  this->init();
}

void MainWindow::on_gameListTableWidget_cellPressed(int row, int /* column */)
{
  for (std::size_t i = 0; i < m_autoControlWidgets.size(); i++)
  {
    auto widget = ui->autoControlTabWidget->widget(i);
    if (widget == nullptr)
    {
      continue;
    }

    if (auto castWidget = dynamic_cast<TabAbstract*>(widget))
    {
      auto currentGW = m_gamesWindowInfo.at(row);
      castWidget->setCurrentGameWindowInfo(currentGW);
      castWidget->onGameWindowInfoPressed(currentGW);
    }
    else
    {
      qDebug() << "Not found control tab";
    }

  }
}
