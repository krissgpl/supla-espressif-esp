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
			FLASH_SIZE=4096
			break
            ;;
        2)
            BOARD=k_dimmer
			FLASH_SIZE=4096
			break
            ;;
        3)
            BOARD=k_switch_dual
			FLASH_SIZE=4096
			break
            ;;
  esac
done

PLIK="$BOARD"_user1."$FLASH_SIZE"_DIO.new.6.sdk3x.bin;
PLIK2="$BOARD"_user2."$FLASH_SIZE"_DIO.new.6.sdk3x.bin;

rm -f /CProjects/supla-espressif-esp/firmware/$PLIK
rm -f /CProjects/supla-espressif-esp/firmware/$PLIK2
	

./build.sh "$BOARD"

if [ -e /CProjects/supla-espressif-esp/firmware/$PLIK ]
then

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
		
	if [ -e /CProjects/supla-espressif-esp/firmware/$PLIK2 ]
	then	

	dialog --clear --backtitle "Podpisanie firmware dla $BOARD" --yesno "Czy podpisac firmware dla plytki $BOARD ?" 10 40
		YOUR_CHOOSE=$?;
		if [ "$YOUR_CHOOSE" == 0 ];
		then
			echo "Podpisanie firmware dla $BOARD";
			echo "Firmware : $PLIK";
			echo "Firmware2 : $PLIK2";
			cd  /CProjects/supla-espressif-esp/firmware
			supla-esp-sigtool -k klucz -s $PLIK
			supla-esp-sigtool -k klucz -s $PLIK2
			supla-esp-sigtool -k klucz -v $PLIK
			if [ command | grep -q verified ];
			then
				echo "Firmware $PLIK podpisane prawidlowo";
			else
				echo "Nie udalo sie podpisac firmware $PLIK !";
			fi
		elif [ "$YOUR_CHOOSE" == 1 ];
		then
			echo "Wybrałeś Nie";
			exit;
		else
			echo "Niczego nie wybrałeś";
			exit;
		fi
	else
		dialog --clear --msgbox "Nie udalo sie skompilowac $BOARD user2 ! Sprawdz log." 10 40
		exit
	fi
		
else
	dialog --clear --msgbox "Nie udalo sie skompilowac $BOARD user1 ! Sprawdz log." 10 40
fi