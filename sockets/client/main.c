#include "main.h"

//*****************************************************************************
//
//*****************************************************************************
static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct FtpFile *out = (struct FtpFile *)stream;
  if(!out->stream) {
    out->stream = fopen(out->filename, "wb");
    if(!out->stream)
      return 0;
  }
  return fwrite(buffer, size, nmemb, out->stream);
}
//*****************************************************************************
//Функция замена прошивки по фтп
//*****************************************************************************
int burn(void)
{
  CURL *curl;
  CURLcode res;
  struct FtpFile ftpfile = {"vend",NULL};
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if(curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL,"ftp://81.26.82.47/vend");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if(CURLE_OK != res)
    {
      fprintf(stderr, "curl told us %d\n", res);
      WriteErrMes("curl told us",1);
    } else {res=0;}
  }

  if(ftpfile.stream) fclose(ftpfile.stream);
  curl_global_cleanup();
  return res;
}

//*****************************************************************************
//Функция получения через сокет и обработки сообщений
//*****************************************************************************
void *receive_messages(void *arg)
{
  char buffer[BUFFERSIZE];
  char SN[128];           // Серийный номер терминала
  char NumApp[5];         // Номер Аппарата
  char Address[128];      // Адресс аппарата
  char query[1024];
  char str[30];
  int k,j;


  while (1)
  {
     memset(buffer, 0, BUFFERSIZE); delay(50);
     ssize_t bytes_received = recv(socket1, buffer, sizeof(buffer), 0);
     if (bytes_received <= 0)
     {
        printf("Server disconnected\n");
        close(socket1);
        del(1);
        F_CLIENT_SEND=0;      // флаг завершения поточной функции отправки сообщений через сокет
        F_CLIENT=1;           // флаг запуска поточной функции переподключения к серверу через сокет
        ArrOfErr[F_SERVER]=1; // Флаг ошибки связи с сервером
        WriteErrMes("Error connection with server",0);
        break;
     }
     else
     {
        if (strncmp(buffer, "ok", 2) == 0)   {delay(100); sendall(socket1,"ok",strlen("ok"),0);}

        if (strncmp(buffer, "BURN", 4) == 0)
        {
            delay(300);
            if(remove("/home/evgen/ALLMOST/vend"))
            {
                printf("Error removing file");
                WriteErrMes("Error removing file",1);
            }

            del(2);
            burn();
            del(2);
            {
                if (!chmod("/home/evgen/ALLMOST/vend",755))                        //     S_IRWXO|S_IRWXG|S_IRWXU
                {
                   WriteErrMes("sowtware firmvare is new, now vend device will be reboot",1); del(1);
                   sync(); reboot(RB_AUTOBOOT);
                }
                else
                {
                   WriteErrMes("Can't change burn",1);    // пишем смс и не пишем в бд флаг 1
                }
            }

        }

        if (strncmp(buffer, "PR", 2) == 0)   // Обновили значение продуктов на сервере
        {
           char Prod[4];
           k=0; j=1;
           for(int i=3;i<strlen(buffer);i++)
           {
             if(buffer[i]!='_') {Prod[k++]=buffer[i];}
             else
             {
                AvailProd[j++]=atoi(Prod);
                k=0; memset(Prod,0,4);
                if(j>4) break;
             }
           }
           WriteAvailProd();
           F_SENS_AVAIL_PROD=1;   // Выставляем флаг проверки наличия продуктов
        }

        if (strncmp(buffer,"SN",2) == 0)     // Заменили серийный номер терминала на сервере
        {
          translit(buffer);
          memset(SN,0,strlen(SN));
          memset(NumApp,0,strlen(NumApp));
          memset(Address,0,strlen(Address));
          memset(query,0,strlen(query));
          ReadMysqlTable("SELECT ADDRESS FROM INF_APP WHERE ID=1",Address);
          ReadMysqlTable("SELECT NUM_APP FROM INF_APP WHERE ID=1",NumApp);
          memmove (&SN[0], &buffer[3], strlen(buffer));
          sprintf(query,"REPLACE INTO INF_APP(ID,NUM_APP,SN_TER,ADDRESS) VALUES('1','%s','%s','%s')",NumApp,SN,Address);
          InsertMysqlTable(query);
        }

        if (strncmp(buffer,"ADDR",4) == 0)    // Заменили адрес местонахождения аппарата на сервере
        {
          translit(buffer);
          memset(SN,0,strlen(SN));
          memset(NumApp,0,strlen(NumApp));
          memset(Address,0,strlen(Address));
          memset(query,0,strlen(query));
          ReadMysqlTable("SELECT SN_TER FROM INF_APP WHERE ID=1",SN);
          ReadMysqlTable("SELECT NUM_APP FROM INF_APP WHERE ID=1",NumApp);
          memmove (&Address[0], &buffer[5], strlen(buffer));
          sprintf(query,"REPLACE INTO INF_APP(ID,NUM_APP,SN_TER,ADDRESS) VALUES('1','%s','%s','%s')",NumApp,SN,Address);
          InsertMysqlTable(query);
        }

        if (strncmp(buffer,"TEL",3) == 0)     // изменили (добавили удалили) номера телефонов сервисной службы
        {
          char Tel[13];
          int Countnumber;
          k=0; j=1; Countnumber=0;
          memset(Tel,0,strlen(Tel));
          memset(query,0,strlen(query));
          InsertMysqlTable("TRUNCATE NUM_TEL");
          for(int i=4;i<strlen(buffer);i++)
          {
           if(buffer[i]!='_') {Tel[k++]=buffer[i];}
           else
           {
             k=0;
             Countnumber++;
             sprintf(query,"REPLACE INTO NUM_TEL(ID,NUMBER_TEL) VALUES('%d','%s')",Countnumber,Tel);
             InsertMysqlTable(query);
             memset(Tel,0,strlen(Tel));
             memset(query,0,strlen(query));
           }
          }
        }

       if (strncmp(buffer, "REBOOT", strlen("REBOOT")) == 0)   // Команда на перезагрузку аппарата
       {
         WriteErrMes("Vend device now will be reboot",1);
         del(1); sync(); reboot(RB_AUTOBOOT);
       }

       if ((strncmp(buffer, "SW_OFF_T", strlen("SW_OFF_T")) == 0)&&!F_SW_OFF_T) // Команда на перезагрузку терминала оплаты
       {
              F_SW_OFF_T=1;     //  Запрет на работу с терминалом в момент перезагрузки
              DW(RES_TER,HIGH); delay(2000);
              DW(RES_TER,LOW);delay(3000);
              F_SW_OFF_T=0;
              WriteErrMes("Terminal reboot is ok ",1);
       }

       if ((strncmp(buffer, "SW_OFF_T", strlen("SW_OFF_T")) == 0)&&F_SW_OFF_T)
       {
         WriteErrMes("Can't reboot terminal, now terminal is busy",1);
       }

       printf("%s\n",buffer);
       memset(buffer, 0, BUFFERSIZE); delay(50);
     }
  }
}

//*****************************************************************************
//Функция отправки сообщений(ошибок и продуктов) через сокет
//*****************************************************************************
void *send_message(void *arg)
{
    int countTime=25000;
    char mes[1024];
    while (1)
    {
       delay(2);
       switch (flag_message)
       {
           case 0: { if (!countTime) { countTime=25000; sendall(socket1,"en",strlen("en"),0);}  break;} // "Поддержка" связи переодической отправкой сообщений на сервер
           case 1: {
                      memset(mes,0,strlen(mes));CodeNumber(mes);
                      sendall(socket1,mes,strlen(mes),0);    // Отправка текущих ошибок закодированных в число
                      printf("%s\n",mes);
                      flag_message=0;
                    } break;
           case 2: {
                      delay(50);memset(mes,0,1024);
                      sprintf(mes,"PR_%d_%d_%d_%d_",AvailProd[1],AvailProd[2],AvailProd[3],AvailProd[4]);
                      sendall(socket1,mes,strlen(mes),0);    // Отправка текущих продуктов после продажи
                      printf("%s\n",mes);
                      flag_message=0;
                    } break;
        }
        countTime--;
        if(!F_CLIENT_SEND) {F_CLIENT_SEND=1;break;} // если обрыв связи с сервером выходим из поточной функции
    }
}
/************************************************************************/
//
/************************************************************************/
struct sockaddr_in enterSocketAddress(char *ip, char *port)
{
    struct sockaddr_in sa;
    inet_pton(AF_INET, ip, &(sa.sin_addr));
    sa.sin_port = htons(atoi(port));
    sa.sin_family = AF_INET;
    return sa;
}
/************************************************************************/
//
/************************************************************************/
void* Client (void *args)
{
   char inf_app[]= "SELECT * FROM INF_APP ORDER BY ID DESC LIMIT 1";
   char NAMECLIENT[10];
   int i,j;        //счетчики

   while(1)
   {
     if(F_CLIENT)  // если не разу еще не соединялись или при обрыве связи
     {
       printf("(Re) Conecting...\n");
       memset(NAMECLIENT,0,sizeof(NAMECLIENT));i=0;j=2;
       ReadMysqlTable(inf_app,ArrForCopy);  // Читаем инфу аппарата из БД
       while (ArrForCopy[j]!=' '){NAMECLIENT[i++]=ArrForCopy[j++]; if (i>4) break;}
       if(strlen(NAMECLIENT)==1) {NAMECLIENT[2]=NAMECLIENT[0];NAMECLIENT[1]='0';NAMECLIENT[0]='0';}
       if(strlen(NAMECLIENT)==2) {NAMECLIENT[2]=NAMECLIENT[1];NAMECLIENT[1]=NAMECLIENT[0];NAMECLIENT[0]='0';}
       memset(ArrForCopy,0,sizeof(ArrForCopy));
       socket1 = Socket(AF_INET, SOCK_STREAM, 0);
       if (socket1 ==-1)
       {
          if (!ArrOfErr[F_SERVER])
          {
           ArrOfErr[F_SERVER]=1;
           WriteErrMes("Error connection with server",0);
          }
          del(10); continue;
       }
       struct sockaddr_in serverAddress = enterSocketAddress("81.26.82.47", "65150");
       if(Connect(socket1, (struct sockaddr *)&serverAddress, sizeof(serverAddress))<0)
       {
         if (!ArrOfErr[F_SERVER])
         {
           ArrOfErr[F_SERVER]=1;
           WriteErrMes("Error connection with server",0);
         }
         del(10);close(socket1); continue;
       }
       send(socket1, NAMECLIENT, strlen(NAMECLIENT), 0);
       char ans[2];
       recv(socket1, ans, sizeof(ans), 0);
       if (ans[0]==NO_PLACE)
       {
         printf("No place on server \n");
         if (!ArrOfErr[F_SERVER])
         {
           ArrOfErr[F_SERVER]=1;
           WriteErrMes("Error connection with server No place on server",0);
         }
         {del(10); continue;}
       }
       printf("Welcome on server!\n");
       F_CLIENT=0;
       pthread_create(&receive_thread, NULL, receive_messages, &socket1);
       pthread_create(&send_thread, NULL, send_message, &socket1);

       delay(700);
       flag_message=1; delay(700); // При подключении к серверу отправляем все текущие ошибки аппарата
       flag_message=2; delay(700); // При подключении к серверу отправляем количество продуктов
       if (ArrOfErr[F_SERVER])
       {
         ArrOfErr[F_SERVER]=0;
         WriteErrMes("Connection with server is ok",0);
         flag_message=1; // Ещё раз отправляем ошибки с учетом ошибки обрыва связи с сервером
       }
       pthread_detach(receive_thread);
       pthread_detach(send_thread);
     }
     del(10);      // каждые 10 сек пытаемся соединиться при обрыве связи
  }
}

/************************************************************************/
//
/************************************************************************/
void* Terminal (void *args)
{
   int  fd;               // переменная для хранения дескриптора ком порта
   int  i=0,j=0;          // счетчики
   int  count_time=40001; // Счетчик времени (для периодической подачи IDL на терминал)


   // если нет ошибок ДКП при запуске программы актуаторы все в выдвинутое положение
  // for(i=3;i==3;i++)
   i=2;
   {F_SELLING=1; if(!ArrOfErr[2*i+3]&&!ArrOfErr[2*i+4]) {Actuator(i,HIGH);} F_SELLING=0;}


   while(1)
   {

     delay(2);
     count_time++;

     //Терминал в состояние IDLE переодически (раз в 80 сек)
     if (count_time>40000&&!ArrOfErr[F_TRAY]&&!ArrOfErr[F_POWER]&&!F_SW_OFF_T)
     {
    //  system("clear");
        F_SW_OFF_T=1;       //запрет на перезагрузку терминала
        count_time=0;
        fd=TryConnection(1);
        if (fd>=0) serialClose(fd);
        F_SW_OFF_T=0;
     }

     // Если "пропало" питание или датчик лотка неисправен
     // Терминал в состояние DIS периодически раз 5 сек
     if(ArrOfErr[F_TRAY]&&ArrOfErr[F_POWER]&&!F_SW_OFF_T)
     {
    //  system("clear");
        F_SW_OFF_T=1;           //запрет на перезагрузку терминала
        count_time=40001;
        fd=TryConnection(2);
        if (fd>=0) serialClose(fd);
        F_SW_OFF_T=0;
     }

      //Если товар отсутствует и нет запретов
     if(F_MISSING&&!ArrOfErr[F_TRAY]&&!ArrOfErr[F_POWER]&&!ArrOfErr[F_SERIAL]&&!ArrOfErr[F_TERMIN]&&!F_SW_OFF_T)
     {
        system("clear");
        F_SW_OFF_T=1;        //запрет на перезагрузку терминала
        F_MISSING=0;
        dfplayer_SetTrakNumber(2);
        fd=TryConnection(3);
        if (fd>=0) serialClose(fd);
        F_SW_OFF_T=0;
     }

     //Если актуатор неисправен и нет запретов
     if(F_ACTUATOR&&!ArrOfErr[F_TRAY]&&!ArrOfErr[F_POWER]&&!ArrOfErr[F_SERIAL]&&!ArrOfErr[F_TERMIN]&&!F_SW_OFF_T)
     {
    //   system("clear");
        F_SW_OFF_T=1;          //запрет на перезагрузку терминала
        F_ACTUATOR=0;
        dfplayer_SetTrakNumber(8);
        fd=TryConnection(5);
        if (fd>=0) serialClose(fd);
        F_SW_OFF_T=0;
     }

     //Если есть товар и нет запретов
     if (F_SELLING&&!ArrOfErr[F_TRAY]&&!ArrOfErr[F_POWER]&&!ArrOfErr[F_SERIAL]&&!ArrOfErr[F_TERMIN]&&!F_SW_OFF_T)
     {
     //  system("clear");
         F_SW_OFF_T=1;         //запрет на перезагрузку терминала
AGAIN:   fd=TryConnection(4);
         if (fd<0) continue;
         dfplayer_SetTrakNumber(1);

         switch (NUM_BUT)
         {
           case 1:{gen_VRP(&req,PRICE1,IDITEM1,NAME1);break;}
           case 2:{gen_VRP(&req,PRICE2,IDITEM2,NAME2);break;}
           case 3:{gen_VRP(&req,PRICE3,IDITEM3,NAME3);break;}
           case 4:{gen_VRP(&req,PRICE4,IDITEM4,NAME4);break;}
         }
         sendUART(&req,fd); delay(100);
         F_SELLING=0; //Разрешаем выбор товара

         while (!read_from_UART(&req,fd)&&!F_SELLING&&!F_MISSING&&!ArrOfErr[F_POWER]&&!F_ACTUATOR)
         {delay(100);i++;if(i>120){i=0;printf("Terminal not answer \n");break;}} i=0;

         if(F_SELLING||F_MISSING||ArrOfErr[F_POWER]||F_ACTUATOR)
         {
             clearR(&req); gen_ABR(&req); sendUART(&req,fd);
             while (!read_from_UART(&req,fd))
             {delay(100);i++; if(i>30){i=0;printf("Terminal not answer \n");break;}} i=0;

             if(ArrOfErr[F_POWER]) //если пропало питание
             {
               F_MISSING=0;F_SELLING=0;NUM_BUT=0;
             }
             if(F_MISSING)  NUM_BUT=0;
             if(F_ACTUATOR) NUM_BUT=0;
             if (fd>=0) serialClose(fd);
             F_SW_OFF_T=0;
             continue;
         }
         else {F_SELLING=1;} del(1);// запрещаем выбор товара


         if (chekSEAL(&req))                  // если оплата прошла успешно
         {
            dfplayer_SetTrakNumber(4); del(2);// товар оплачен
            get_brecpt(receipt,&req); // получаем чек
            dfplayer_SetTrakNumber(5); del(2);// электронный чек на экране терминала
            gen_IDL_QR(&req, receipt);sendUART(&req,fd); // qr cod на экран терминала

            //Выдаем товар задвигая и затем выдвигая соответствующий актуатор
            Actuator(NUM_BUT,LOW); gen_IDL_QR(&req, receipt);sendUART(&req,fd);
            Actuator(NUM_BUT,HIGH);gen_IDL_QR(&req,receipt); sendUART(&req,fd);
            del(5);//Время ожидания падения товара

            if (!DSR[S_TRAY]) //Если не сработал датчик в лотке выдачи
            {
               dfplayer_SetTrakNumber(7);// При выдаче товара возникла ошибка деньги будут автоматически возвращены
               switch (NUM_BUT)
               {
                 case 1:{gen_FIN(&req,0,IDITEM1,NAME1);break;}
                 case 2:{gen_FIN(&req,0,IDITEM2,NAME2);break;}
                 case 3:{gen_FIN(&req,0,IDITEM3,NAME3);break;}
                 case 4:{gen_FIN(&req,0,IDITEM4,NAME4);break;}
               }
               sendUART(&req,fd);delay (100);
               while (!read_from_UART(&req,fd)){delay(100);i++; if(i>70){i=0;printf("Terminal not answer \n");break;}} i=0; del(3);

               if(!ArrOfErr[2*NUM_BUT+3]&&!ArrOfErr[2*NUM_BUT+4])
               {
                 gen_DIS(&req); sendUART(&req,fd);delay (100);
                 while (!read_from_UART(&req,fd)){delay(100);i++;
                 if(i>50){i=0;printf("Terminal not answer \n");break;}} i=0;
                 ArrOfErr[F_TRAY]=1; // здесь выставляем флаг запрета работы аппарата по датчику лотка
                 flag_message=1;     // Шлем на сервер ошибку
                 WriteErrMes("Error tray sensor",0);   //пишем ошибку в БД и смс
                 NUM_BUT=0; F_SELLING=0;
               }

               if (ArrOfErr[2*NUM_BUT+3]||ArrOfErr[2*NUM_BUT+4])
               {
                   COUNT_OF_ERROR_ACT++;
                  /* если в момент выдачи товара
                    ДКП (датчик конечного положения) актуатора не сработал тогда в первый раз
                    не помечаем ошибку датчика лотка выдачи. Если последовательно!!! будет
                    второй раз ошибка актуатора тогда выставляем флаг ошибки. При
                    непоследовательных ошибках ДКП счетчик сбрасывается*/

                  if(COUNT_OF_ERROR_ACT>1)
                  {
                    COUNT_OF_ERROR_ACT=0;
                    gen_DIS(&req); sendUART(&req,fd);delay (100);
                    while (!read_from_UART(&req,fd)){delay(100);i++;
                    if(i>70){i=0;printf("Terminal not answer \n");break;}} i=0; del(3);
                    ArrOfErr[F_TRAY]=1; // здесь выставляем флаг запрета работы аппарата по датчику лотка
                    flag_message=1;     // Шлем на сервер
                    WriteErrMes("Error tray sensor",0);   // пишем ошибку в БД и смс
                  }
                  NUM_BUT=0; F_SELLING=0;
               }
            }

            else // иначе финализируем покупку
            {
                gen_IDL_SCREEN_TAKE(&req);sendUART(&req,fd);delay(2500);
                DW(ELEC_LOCK,HIGH);        // Открываем замок
                //  DW(REVERS,HIGH);
                //  DW(ELEC_LOCK,LOW);
                switch (NUM_BUT)
                {
                  case 1:{
                           gen_FIN(&req,PRICE1,IDITEM1,NAME1);
                           sprintf(ArrForCopy,"INSERT INTO CHECK_(CH1,CH2,CH3,CH4,CH_ERR,DTIME) VALUES('%s', '', '', '', '', NOW())",translit(receipt));
                           InsertMysqlTable(ArrForCopy);   // запись чека в БД
                           break;
                          }
                  case 2:{
                           gen_FIN(&req,PRICE2,IDITEM2,NAME2);
                           sprintf(ArrForCopy,"INSERT INTO CHECK_(CH1,CH2,CH3,CH4,CH_ERR,DTIME) VALUES('','%s', '', '', '', NOW())",translit(receipt));
                           InsertMysqlTable(ArrForCopy);   // запись чека в БД
                           break;
                          }
                  case 3:{
                           gen_FIN(&req,PRICE3,IDITEM3,NAME3);
                           sprintf(ArrForCopy,"INSERT INTO CHECK_(CH1,CH2,CH3,CH4,CH_ERR,DTIME) VALUES('','', '%s', '', '', NOW())",translit(receipt));
                           InsertMysqlTable(ArrForCopy);  // запись чека в БД
                           break;
                         }
                  case 4:{
                           gen_FIN(&req,PRICE4,IDITEM4,NAME4);
                           sprintf(ArrForCopy,"INSERT INTO CHECK_(CH1,CH2,CH3,CH4,CH_ERR,DTIME) VALUES('','', '', '%s', '', NOW())",translit(receipt));
                           InsertMysqlTable(ArrForCopy);  // запись чека в БД
                           break;
                         }
                }
                sendUART(&req,fd);delay (100);
                while (!read_from_UART(&req,fd)){delay(100);i++; if(i>120){i=0;printf("Terminal not answer \n");break;}} i=0;
                dfplayer_SetTrakNumber(6); // Заберите товар
                while (DSR[S_TRAY])  // Пока не забрали товар ждем
                {
                  del(1);i++;
                  if((i==6)||(i==12)){dfplayer_SetTrakNumber(6);}
                  if ((i % 4)==0)
                  {
                    system("clear");
                    gen_IDL_SCREEN_TAKE(&req); sendUART(&req,fd);
                  }
                  if (i==180) {WriteErrMes("Buyer did not pick up product",1);} // Если товар не забрали в течении 3 минут
                                                                                // пишем смс и не пишем в бд флаг 1
                }

                ReadAvailProd();     // Читаем наличие продуктов во всех секциях
                AvailProd[NUM_BUT]=AvailProd[NUM_BUT]-1;
                F_SENS_AVAIL_PROD=1; // Выставляем флаг проверки датчиков наличия товара
                flag_message=2;      // Отправляем текущее количество продуктов через сокет
                if (AvailProd[NUM_BUT]==9)   // Если количество товара в соответствующей секции меньше 10 сообщаем 1 раз
                {
                  char str[30];
                  sprintf(str,"Product %d %s",NUM_BUT,"less than 10");
                  WriteErrMes(str,1);   // пишем смс и не пишем в бд флаг 1
                }

                if (AvailProd[NUM_BUT]==0)  // Если количество товара в соответствующей секции 0 сообщаем
                {
                  char str[30];
                  sprintf(str,"Product %d %s",NUM_BUT,"is end");
                  WriteErrMes(str,1);    // пишем смс и не пишем в бд флаг 1
                }
                WriteAvailProd();

                i=0; del(5);      // время ожидания до закрытия замка на лотке выдачи  5 sek
                NUM_BUT=0; F_SELLING=0; count_time=40001;
                COUNT_OF_ERROR_ACT=0;
                DW(ELEC_LOCK,LOW); // Закрываем замок
                //  DW(REVERS,LOW);
                //  DW(ELEC_LOCK,HIGH);
             }
           }

           // иначе если на карте нет средств или не производили оплату
      else {
               if(get_brecpt(receipt,&req))  // если нет средств
               {
                 sprintf(ArrForCopy,"INSERT INTO CHECK_(CH1,CH2,CH3,CH4,CH_ERR,DTIME) VALUES('','', '', '', '%s', NOW())",translit(receipt));
                 InsertMysqlTable(ArrForCopy);     // пишем чек с ошибкой в БД
                 dfplayer_SetTrakNumber(3);del (2);// Товар не оплачен попробуйте еще раз
                 if (fd>=0) serialClose(fd); delay(300);
                 goto AGAIN;
               }
               gen_ABR(&req);
               sendUART(&req,fd);del(2);
               count_time=40001;
               NUM_BUT=0; F_SELLING=0;
           }
           if (fd>=0) serialClose(fd);
           F_SW_OFF_T=0;
      }//end if seling
    }//end while
}//end mytread func */





int main()
{
  int i,k;        //счетчики
  int  button[5]={0};
  int  sensor[5]={0};
  pthread_t TermThread, ClThread;

  del(10);               // Задержка при перезагрузке для того чтобы успели загрузиться все нужные сервисы и службы
  InOutInit();           // инициализация портов ввода вывода
  dfplayer_Init();       // инициализация плеера для звуковых оповещений

  ReadFlagErr();         // Читаем значения флагов ошибок при запуске
  ReadAvailProd();       // Читаем наличие продуктов в соответствующих секциях

  WriteErrMes("Welcome to vend device",1);

  int status = pthread_create(&TermThread, NULL, Terminal, NULL);     // Поток для работы с терминалом оплаты
  if (status != 0) {
    printf("main error: can't create thread, status = %d\n", status);
    exit(ERROR_CREATE_THREAD);
   }

  status = pthread_create(&ClThread, NULL, Client, NULL);     // Поток для работы с сокетами
  if (status != 0) {
    printf("main error: can't create thread, status = %d\n", status);
    exit(ERROR_CREATE_THREAD);
   }
   printf("dfvddgfwefg\n");

 while (1)
 {
   // Читаем постоянно состояние всех кнопок и датчиков со сдвигового регистра
   myDigRead (DSR); delay(80);

   button[1]=DSR[BUT_1];sensor[1]=DSR[S_PROD_1];
   button[2]=DSR[BUT_2];sensor[2]=DSR[S_PROD_2];
   button[3]=DSR[BUT_3];sensor[3]=DSR[S_PROD_3];
   button[4]=DSR[BUT_4];sensor[4]=DSR[S_PROD_4];

   if(DSR[RES_1]) // обнуляем все ошибки аппарата кроме отсутствия питания
   {
     delay(200);
     for (i=1;i<17;i++) {ArrOfErr[i]=0;}
     flag_message=1;
     COUNT_OF_ERROR_ACT=0;
     WriteErrMes("All error off",0);
   }

   if(DSR[RES_2])          // проверка датчиков наличия товара в секциях
   {
     delay(200);
     ReadAvailProd();      // Читаем наличие  продуктов в соответствующих секциях
     F_SENS_AVAIL_PROD=1;  // Выставляем флаг проверки датчика наличия продукта в секциях
   }

   if(DSR[RES_3])          // Резерв
   {

   }


   if(DSR[RES_4]) // снимаем блокировку неисправных датчиков актуаторов
   {
     for (i=5;i<13;i++) {ArrOfErr[i]=0;}
     flag_message=1;
     COUNT_OF_ERROR_ACT=0;
     WriteErrMes("All actuator error off",0);
   }


   if (F_SENS_AVAIL_PROD)
   {
     F_SENS_AVAIL_PROD=0;
     for(k=1;k<5;k++)
     {
       char str[30];
       if ((!sensor[k]&&!ArrOfErr[k+12]&&AvailProd[k])||(sensor[k]&&!ArrOfErr[k+12]&&!AvailProd[k]))
       {
          ArrOfErr[k+12]=1; snprintf(str,30,"Sensor product %d %s",k,"error");
          flag_message=1;  WriteErrMes(str,0);
       }

       if ((sensor[k]&&ArrOfErr[k+12]&&AvailProd[k])||(!sensor[k]&&ArrOfErr[k+12]&&!AvailProd[k]))
       {
          ArrOfErr[k+12]=0; snprintf(str,30,"Sensor product %d %s",k,"ok");
          flag_message=1;  WriteErrMes(str,0);
       }
     }
    }


    // "Подсвечиваем" кнопки в соотвествии со всеми флагами и условиями
    for(k=0;k<4;k++)
    {
     if (ArrOfErr[0]||ArrOfErr[1]||ArrOfErr[2]||ArrOfErr[4]) {RGB(RED,ALL); break;}
     if(!sensor[k+1]||!AvailProd[k+1]||ArrOfErr[k*2+5]||ArrOfErr[k*2+6]||ArrOfErr[k+13]) {RGB(RED,k+1);}
     else {k==(NUM_BUT-1)?RGB(GREEN,k+1):RGB(WHITE,k+1);}
    }


    if(digitalRead(AVAIL_POWER))     // мониторим вход наличия питания
    {
      if(ArrOfErr[F_POWER])          // при появлении питания после "пропадания"
      {
        del(1);
        dfplayer_SetTrakNumber(10);  // внешнее электропитание восстановлено
        ArrOfErr[F_POWER]=0;         // обнуляем флаг ошибки
        flag_message=1;              // отправляем на сервер
        WriteErrMes("Power on",0);   // Пишем смс и в БД
      }

      k=4;
      while ((k>=1)&&(!F_SELLING)&&(!DSR[S_TRAY])&&!ArrOfErr[F_TRAY]&&!ArrOfErr[F_SERIAL]&&!ArrOfErr[F_TERMIN])
      {
        if(button[k])
         {
              for (i=0;i<4;i++) {LowRgb(k); delay(200);RGB(WHITE,k);delay(200);}// мигаем белым цветом выбранной кнопки

              if(!sensor[k]||!AvailProd[k]) // Если датчик наличия продукта не сработал зажигаем красный и уходим по флагу
              {

                if(ArrOfErr[k*2+3]||ArrOfErr[k*2+4]) // ДКП актуаторов
                {
                  F_ACTUATOR=1; // соответствующий актуатор неисправен (уходим по флагу в функцию terminal)
                }
                else
                {
                  RGB(RED,k);  // Красный цвет кнопки
                  F_MISSING=1; // выставляем флаг отсутствия товара (уходим по флагу в функцию terminal)
                }
              }
              else
              {
                if(ArrOfErr[k*2+3]||ArrOfErr[k*2+4]) // ДКП актуаторов
                {
                  F_ACTUATOR=1; // соответствующий актуатор неисправен (уходим по флагу в функцию terminal)
                }
                else
                {
                  RGB(GREEN,k); // Зеленый цвет кнопки
                  NUM_BUT=k;    // Запоминаем выбранную кнопку
                  F_SELLING=1;  // выставляем флаг покупки   (уходим по флагу в функцию terminal)
                }
              }
              k=0;
          }
          k--;
       }
    }//end if power avial

    else {
             del(5); // задержка от кратковременных скачков напряжения
             if(!ArrOfErr[F_POWER]&&!digitalRead(AVAIL_POWER)) //если питание отсутствует
             {
                dfplayer_SetTrakNumber(9);  // внешнее электропитание отключено
                ArrOfErr[F_POWER]=1;        // взводим флаг ошибки
                flag_message=1;             // отправляем на сервер
                WriteErrMes("Power off",0); // Пишем смс и в БД
             }
         } //end if else power avial    */
  } //end while

     pthread_join(TermThread, NULL);
     pthread_join(ClThread, NULL);

} //end main

/**************************************************************************/

/**************************************************************************/
