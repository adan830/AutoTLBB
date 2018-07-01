#ifndef GAMEWINDOWINFO_HPP
#define GAMEWINDOWINFO_HPP

#include "../player_control_tabs/GeneralTab.hpp"
#include "../Preprocessor.hpp"

#include <Windows.h>
#include <QDebug>

class Player;
class Settings;

class GameWindowInfo
{
  public:
    GameWindowInfo();
    ~GameWindowInfo();

  public:
    void sendMessage(const UINT msg, const WPARAM wParam, const LPARAM lParam) const;
    void postMessage(const UINT msg, const WPARAM wParam, const LPARAM lParam) const;
    void setWindowName(const QString& name);

  public:
    template<typename T, typename Adr, typename ...Adrs>
    inline T readMemory(const Adr& adr, const Adrs& ...address) const
    {
      std::vector<Adr> adrs = {adr, address...};

      DWORD nextOffset = *adrs.cbegin();
      for (std::size_t i = 1; i < adrs.size(); i++)
      {
        ::ReadProcessMemory(
            this->m_handle,
            reinterpret_cast<LPCVOID>(nextOffset + adrs.at(i)),
            reinterpret_cast<LPVOID>(&nextOffset),
            sizeof(DWORD), nullptr
            );
      }

      T value;
      ::ReadProcessMemory(
            this->m_handle,
            reinterpret_cast<LPCVOID>(nextOffset),
            reinterpret_cast<LPVOID>(&value),
            sizeof(T), nullptr
            );
      return value;
    }

  private:
    APP_SYNTHESIZE(HWND, m_hwnd, Hwnd)
    APP_SYNTHESIZE(HANDLE, m_handle, Handle)
    APP_SYNTHESIZE(DWORD, m_processId, ProcessId)
    APP_SYNTHESIZE_PTR(Player, m_player, Player)
    APP_SYNTHESIZE_PTR(Settings, m_settings, Settings)

    private:
      friend QDebug operator<<(QDebug qdb, const GameWindowInfo& gameWindowInfo);
};

#endif // GAMEWINDOWINFO_HPP
