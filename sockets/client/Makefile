opi_pc: main.o dfplayer.o piHiPri.o piThread.o softPwm.o softTone.o terminal.o wiringPi.o wiringSerial.o socketFunctions.o
	gcc main.o dfplayer.o piHiPri.o piThread.o softPwm.o softTone.o terminal.o wiringPi.o wiringSerial.o socketFunctions.o -o vend -pthread -lmysqlclient -lwiringPi -lcurl

main.o: main.c
	gcc -c main.c

dfplayer.o: dfplayer.c
	gcc -c dfplayer.c

piHiPri.o: piHiPri.c
	gcc -c piHiPri.c

piThread.o: piThread.c
	gcc -c piThread.c

softPwm.o: softPwm.c
	gcc -c softPwm.c

softTone.o: softTone.c
	gcc -c softTone.c

terminal.o: terminal.c
	gcc -c terminal.c

wiringPi.o: wiringPi.c
	gcc -c wiringPi.c

wiringSerial.o: wiringSerial.c
	gcc -c wiringSerial.c

clean:
	rm -rf *.o vend