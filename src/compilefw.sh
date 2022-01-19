#!/bin/bash

DIALOG_CANCEL=1
DIALOG_ESC=255
HEIGHT=15
WIDTH=40
CHOICE_HEIGHT=4
BACKTITLE="SUPLA FIRMWARE COMPILER"
TITLE="PLYTKI"
MENU="Wybierz plytke:"

OPTIONS=(1 "k_rs_module_v3"
         2 "k_dimmer"
         3 "k_switch_dual")

while true; do
	exec 3>&1

		CHOICE=$(dialog --clear \
                --backtitle "$BACKTITLE" \
                --title "$TITLE" \
                --menu "$MENU" \
                $HEIGHT $WIDTH $CHOICE_HEIGHT \
                "${OPTIONS[@]}" \
                2>&1 1>&3)
  exit_status=$?
  exec 3>&-
  case $exit_status in
    $DIALOG_CANCEL)
      clear
      echo "Nie wybrales zadnej plytki !."
      exit
      ;;
    $DIALOG_ESC)
      clear
      echo "Program aborted." >&2
      exit 1
      ;;
  esac
  case $CHOICE in
        1)
            BOARD=k_rs_module_v3
			break
            ;;
        2)
            BOARD=k_dimmer
			break
            ;;
        3)
            BOARD=k_switch_dual
			break
            ;;
  esac
done

./build.sh "$BOARD"

echo "gotowe"

dialog --clear --backtitle "USER2 dla $BOARD" --yesno "Czy skompilowac USER2 dla plytki $BOARD ?" 10 40
    YOUR_CHOOSE=$?;
    if [ "$YOUR_CHOOSE" == 0 ];
    then
        ./build.sh "$BOARD" user2
    elif [ "$YOUR_CHOOSE" == 1 ];
    then
        echo "Wybrałeś Nie";
		exit;
    else
        echo "Niczego nie wybrałeś";
		exit;
    fi

dialog --clear --title "Czy podpisac firmware Tak/nie" --backtitle "USER2" --yesno "Twoj wybor:" 10 40
    YOUR_CHOOSE=$?;
    if [ "$YOUR_CHOOSE" == 0 ];
    then
        ./build.sh "$BOARD" user2
    elif [ "$YOUR_CHOOSE" == 1 ];
    then
        echo "Wybrałeś Nie";
		exit;
    else
        echo "Niczego nie wybrałeś";
		exit;
    fi