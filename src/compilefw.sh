#!/bin/bash

DIALOG_CANCEL=1
DIALOG_ESC=255
HEIGHT=37
WIDTH=40
CHOICE_HEIGHT=29
BACKTITLE="SUPLA FIRMWARE COMPILER"
TITLE="PLYTKI"
MENU="Wybierz plytke:"

OPTIONS=(1 "k_gate_module"
		 2 "k_gate_module_v3"
		 3 "k_dimmer"
		 4 "k_dimmer_din"
		 5 "k_gniazdko_neo"
		 6 "k_rs_module_v3"
		 7 "k_rs_module_v4"
		 8 "k_socket_v2"
		 9 "k_socket_dual_v2"
		 10 "k_switch_dual"
		 11 "k_socket_SSR"
		 12 "k_yunschan"
		 13 "k_socket_01"
		 14 "k_smoke_module"
		 15 "k_smoke_module_ds18b20"
		 16 "k_smoke_module_DHT22"
		 17 "k_socket"
		 18 "k_socket_ds18b20"
		 19 "k_socket_DHT22"
		 20 "k_socket_dual"
		 21 "k_socket_dual_ds18b20"
		 22 "k_socket_dual_DHT22"
		 23 "k_sonoff"
		 24 "k_sonoff_ds18b20"
		 25 "k_sonoff_DHT22"
		 26 "k_sonoff_touch"
		 27 "k_sonoff_touch_dual"
		 28 "k_sonoff_touch_triple"
		 29 "k_sonoff_pow_R2"
		 30 "k_impulse_counter"
		 31 "k_impulse_counter_3"
		 32 "k_versa_module")

rm -f /CProjects/supla-espressif-esp/firmware/result.txt
rm -f /CProjects/supla-espressif-esp/firmware/result2.txt

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
            BOARD=k_gate_module
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
        2)
            BOARD=k_gate_module_v3
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
        3)
            BOARD=k_dimmer
			FLASH_SIZE=4096
			NOSSL=1
			SPI=DIO
			PARAM=6
			break
            ;;
		4)
            BOARD=k_dimmer_din
			FLASH_SIZE=4096
			NOSSL=1
			SPI=DIO
			PARAM=6
			break
            ;;
		5)
            BOARD=k_gniazdko_neo
			FLASH_SIZE=1024
			SPI=DIO
			PARAM=2
			break
            ;;
		6)
            BOARD=k_rs_module_v3
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		7)
            BOARD=k_rs_module_v4
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		8)
            BOARD=k_socket_v2
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		9)
            BOARD=k_socket_dual_v2
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		10)
            BOARD=k_switch_dual
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		11)
            BOARD=k_socket_SSR
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		12)
            BOARD=k_yunschan
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		13)
            BOARD=k_socket_01
			FLASH_SIZE=1024
			SPI=DIO
			PARAM=2
			break
            ;;
		14)
            BOARD=k_smoke_module
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		15)
            BOARD=k_smoke_module_ds18b20
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		16)
            BOARD=k_smoke_module_DHT22
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		17)
            BOARD=k_socket
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		18)
            BOARD=k_socket_ds18b20
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		19)
            BOARD=k_socket_DHT22
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		20)
            BOARD=k_socket_dual
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		21)
            BOARD=k_socket_dual_ds18b20
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		22)
            BOARD=k_socket_dual_DHT22
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
		23)
            BOARD=k_sonoff
			FLASH_SIZE=1024
			SPI=DIO
			PARAM=2
			break
            ;;
		24)
            BOARD=k_sonoff_ds18b20
			FLASH_SIZE=1024
			SPI=DIO
			PARAM=2
			break
            ;;
		25)
            BOARD=k_sonoff_DHT22
			FLASH_SIZE=1024
			SPI=DIO
			PARAM=2
			break
            ;;
		26)
            BOARD=k_sonoff_touch
			FLASH_SIZE=1024
			SPI=DOUT
			PARAM=2
			break
            ;;
		27)
            BOARD=k_sonoff_touch_dual
			FLASH_SIZE=1024
			SPI=DOUT
			PARAM=2
			break
            ;;
		28)
            BOARD=k_sonoff_touch_triple
			FLASH_SIZE=1024
			SPI=DOUT
			PARAM=2
			break
            ;;
		29)
            BOARD=k_sonoff_pow_R2
			FLASH_SIZE=2048
			SPI=DIO
			PARAM=5
			break
            ;;
		30)
            BOARD=k_impulse_counter
			FLASH_SIZE=2048
			SPI=DIO
			PARAM=5
			break
            ;;
		31)
            BOARD=k_impulse_counter_3
			FLASH_SIZE=2048
			SPI=DIO
			PARAM=5
			break
            ;;
		32)
            BOARD=k_versa_module
			FLASH_SIZE=4096
			SPI=DIO
			PARAM=6
			break
            ;;
  esac
done

if [ $NOSSL == 1 ]
then
	PLIK="$BOARD"_nossl_user1."$FLASH_SIZE"_"$SPI".new."$PARAM".sdk3x.bin;
	PLIK2="$BOARD"_nossl_user2."$FLASH_SIZE"_"$SPI".new."$PARAM".sdk3x.bin;
else
	PLIK="$BOARD"_user1."$FLASH_SIZE"_"$SPI".new."$PARAM".sdk3x.bin;
	PLIK2="$BOARD"_user2."$FLASH_SIZE"_"$SPI".new."$PARAM".sdk3x.bin;
fi

echo "PLIK : $PLIK";
echo "PLIK2 : $PLIK2";

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
			supla-esp-sigtool -k klucz -v $PLIK &> result.txt
			supla-esp-sigtool -k klucz -v $PLIK2 &> result2.txt
			if  grep -q 'verified' result.txt
			then
				echo "Firmware $PLIK podpisane prawidlowo";
				mkdir -p /CProjects/supla-espressif-esp/firmware/signed
				rm -f /CProjects/supla-espressif-esp/firmware/signed/$PLIK
				cp /CProjects/supla-espressif-esp/firmware/$PLIK /CProjects/supla-espressif-esp/firmware/signed/$PLIK
				rm -f /CProjects/supla-espressif-esp/firmware/$PLIK
				rm -f /CProjects/supla-espressif-esp/firmware/result.txt
				sleep 1
				dialog --clear --msgbox "Firmware $PLIK podpisane i przeniesione do firmware/signed." 10 40
			else
				echo "Nie udalo sie podpisac firmware $PLIK !";
			fi
			if  grep -q 'verified' result2.txt
			then
				echo "Firmware $PLIK2 podpisane prawidlowo";
				mkdir -p /CProjects/supla-espressif-esp/firmware/signed
				rm -f /CProjects/supla-espressif-esp/firmware/signed/$PLIK2
				cp /CProjects/supla-espressif-esp/firmware/$PLIK2 /CProjects/supla-espressif-esp/firmware/signed/$PLIK2
				rm -f /CProjects/supla-espressif-esp/firmware/$PLIK2
				rm -f /CProjects/supla-espressif-esp/firmware/result2.txt
				sleep 1
				dialog --clear --msgbox "Firmware $PLIK2 podpisane i przeniesione do firmware/signed." 10 40
			else
				echo "Nie udalo sie podpisac firmware $PLIK2 !";
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