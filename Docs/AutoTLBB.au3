Opt("MouseClickDownDelay",80)
Opt("MustDeclareVars",1)

Global $rad[7]
Global $direction=1
dim $i
dim $Form1
dim $cmdGo
dim $cmdBack
dim $cmdIn
dim $nMsg
dim $tlbb1
dim $tlbb2
dim $currentWindow
dim $currentMouse
#include <ButtonConstants.au3>
#include <GUIConstantsEx.au3>
#include <WindowsConstants.au3>
Opt("GUIResizeMode", $GUI_DOCKWIDTH+$GUI_DOCKHEIGHT)
#Region ### START Koda GUI section ### Form=
$Form1 = GUICreate("Auto-Moving (TRAN Nhat-Tuan)", 294, 235, 932, 100)
GUISetIcon("D:\Storage\tuan1.ico", -1)
$cmdGo = GUICtrlCreateButton("&GO", 176, 24, 89, 49)
GUICtrlSetFont(-1, 14, 400, 2, "Times New Roman")
$cmdBack = GUICtrlCreateButton("&BACK", 176, 152, 89, 49)
GUICtrlSetFont(-1, 14, 400, 2, "Times New Roman")
$cmdIn = GUICtrlCreateButton("&IN", 176, 88, 89, 49)
GUICtrlSetFont(-1, 14, 400, 2, "Times New Roman")
$rad[6] = GUICtrlCreateRadio("thanh Lang Nhac", 24, 200, 105, 17)
$rad[5] = GUICtrlCreateRadio("Quynh Chau", 24, 169, 81, 17)
$rad[4] = GUICtrlCreateRadio("Nam Hai", 24, 139, 65, 17)
$rad[3] = GUICtrlCreateRadio("Vo Di", 24, 108, 65, 17)
$rad[2] = GUICtrlCreateRadio("Tay Ho", 24, 77, 65, 17)
$rad[1] = GUICtrlCreateRadio("Long Tuyen", 24, 47, 81, 17)
$rad[0] = GUICtrlCreateRadio("thanh Long Tuyen", 24, 16, 105, 17)
GUICtrlSetState(-1, $GUI_CHECKED)
GUISetState(@SW_SHOW)
WinWait("[CLASS:TianLongBaBu WndClass]","",2)
If 1-WinExists("[CLASS:TianLongBaBu WndClass]") Then exit
#EndRegion ### END Koda GUI section ###

While 1
	$nMsg = GUIGetMsg()
	Switch $nMsg
		Case $GUI_EVENT_CLOSE
			Exit
		Case $cmdGo
			blockinput(1)
			$currentWindow=WinGetHandle("")
			$currentMouse=MouseGetPos ()
			WinActivate("[CLASS:TianLongBaBu WndClass]")
			$direction=1
			for $i=0 to 6
				if BitAND(GUICtrlRead($rad[$i]), $GUI_CHECKED) = $GUI_CHECKED Then
					walk($i)
					MouseMove($currentMouse[0],$currentMouse[1])
					blockinput(0)
					WinActivate($currentWindow)
					ExitLoop
					endif
				next
		Case $cmdBack
			BlockInput(1)
			$currentWindow=WinGetHandle("")
			$currentMouse=MouseGetPos ()
			WinActivate("[CLASS:TianLongBaBu WndClass]")
			$direction=0
			for $i=0 to 6
				if BitAND(GUICtrlRead($rad[$i]), $GUI_CHECKED) = $GUI_CHECKED Then
					walk($i)
					MouseMove($currentMouse[0],$currentMouse[1])
					BlockInput(0)
					WinActivate($currentWindow)
					ExitLoop
					endif
				next
		Case $cmdIn
			BlockInput(1)
			$currentWindow=WinGetHandle("")
			$currentMouse=MouseGetPos ()
			WinActivate("[CLASS:TianLongBaBu WndClass]")
			getin()
			MouseMove($currentMouse[0],$currentMouse[1])
			BlockInput(0)
			WinActivate($currentWindow)
		EndSwitch
WEnd

Func walk($n)
	If $direction=1 Then
		Switch $n
			Case 0	;thanh Long Tuyen
				without_TNCL(100,157)
			Case 1	;Long Tuyen
				TNCL(2)
			Case 2	;Tay Ho
				TNCL(4)
			Case 3	;Vo Di
				TNCL(3)
			Case 4	;Nam Hai
				TNCL(2)
			Case 5	;Quynh Chau
				TNCL(3)
			Case 6	;thanh Lang Nhac
				without_TNCL(149,57)
			EndSwitch
		If $n<6 then GUICtrlSetState($rad[$i+1], $GUI_CHECKED)
	Elseif $direction=0 Then
		Switch $n
			Case 0	;thanh Long Tuyen
				without_TNCL(149,57)
			Case 1	;Long Tuyen
				TNCL(4)
			Case 2	;Tay Ho
				TNCL(1)
			Case 3	;Vo Di
				TNCL(4)
			Case 4	;Nam Hai
				TNCL(3)
			Case 5	;Quynh Chau
				TNCL(1)
			Case 6	;thanh Lang Nhac
				without_TNCL(100,157)
			EndSwitch
		If $n>0 then GUICtrlSetState($rad[$i-1], $GUI_CHECKED)
	EndIf
EndFunc

func without_TNCL($x,$y)
	WinWaitActive("[CLASS:TianLongBaBu WndClass]")
	$tlbb1=WinGetPos("[CLASS:TianLongBaBu WndClass]")
	$tlbb2=WinGetClientSize("[CLASS:TianLongBaBu WndClass]")
	sleep(100)
	send("!`")
	sleep(100)
	mouseclick("left",$tlbb1[0]+575,$tlbb1[1]+495,2)
	send($x)
	mouseclick("left",$tlbb1[0]+615,$tlbb1[1]+495,2)
	send($y)
	send("{F9}")
	mouseclick("left",$tlbb1[0]+655,$tlbb1[1]+495,2)
	sleep(100)
	send("!`")
	endFunc
	
func TNCL($n)
	$n=315+20*$n
	WinWaitActive("[CLASS:TianLongBaBu WndClass]")
	$tlbb1=wingetpos("[CLASS:TianLongBaBu WndClass]")
	$tlbb2=WinGetClientSize("[CLASS:TianLongBaBu WndClass]")
	sleep(100)
	send("!`")
	sleep(100)
	mouseclick("left",$tlbb1[0]+615,$tlbb1[1]+$n,2)
	autoitsetoption("MouseClickDownDelay",5)
	mouseclick("left",$tlbb1[0]+615,$tlbb1[1]+$n,20)
	autoitsetoption("MouseClickDownDelay",80)
	send("{F9}")
	mouseclick("left",$tlbb1[0]+655,$tlbb1[1]+495,2)
	sleep(100)
	send("!`")
	endFunc

func getin()					;g
	WinWaitActive("[CLASS:TianLongBaBu WndClass]")
	$tlbb1=wingetpos("[CLASS:TianLongBaBu WndClass]")
	$tlbb2=WinGetClientSize("[CLASS:TianLongBaBu WndClass]")
	mouseclick("left",$tlbb1[0]+135,$tlbb1[1]+200,2)
	EndFunc
