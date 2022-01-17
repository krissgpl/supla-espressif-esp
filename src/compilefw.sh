#!/bin/bash

HEIGHT=15
WIDTH=40
CHOICE_HEIGHT=4
BACKTITLE="SUPLA FIRMWARE COMPILER"
TITLE="PLYTKI"
MENU="Wybierz plytke:"

OPTIONS=(1 "k_rs_module_v3"
         2 "k_dimmer"
         3 "k_switch_dual")

CHOICE=$(dialog --clear \
                --backtitle "$BACKTITLE" \
                --title "$TITLE" \
                --menu "$MENU" \
                $HEIGHT $WIDTH $CHOICE_HEIGHT \
                "${OPTIONS[@]}" \
                2>&1 >/dev/tty)

clear
case $CHOICE in
        1)
            BOARD=k_rs_module_v3
            ;;
        2)
            BOARD=k_dimmer
            ;;
        3)
            BOARD=k_switch_dual
            ;;
esac

./build.sh "$BOARD"

echo "gotowe"

dialog --clear --title "Czy skompilowac USER2 Tak/nie" --backtitle "USER2" --yesno "Twój wybór:" 10 40
    YOUR_CHOOSE=$?;
    if [ "$YOUR_CHOOSE" == 0 ];
    then
        ./build.sh "$BOARD" user2
    elif [ "$YOUR_CHOOSE" == 1 ];
    then
        echo "Wybrałeś Nie";
    else
        echo "Niczego nie wybrałeś";
    fi
