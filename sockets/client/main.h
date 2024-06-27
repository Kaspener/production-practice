#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <time.h>
#include <wiringPi.h>
#include <softTone.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "settings.h"
#include "dfplayer.h"
#include "terminal.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <mysql/mysql.h>
#include "socketFunctions.h"
#include <curl/curl.h>

#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4
#define ALL 5

#define DATA 7       // (Вход) данных сдвигового регистра 74HC165
#define LATCH 8      // Защелка  (Выход) 74HC165
#define CLOCK 9      // Тактовый сигнал (Выход) 74HC165
#define ELEC_LOCK 27 // (Выход) Электромагнитный замок приемного лотка

#define BUT_4 20 // Кнопка 1 (Вход)
#define BUT_3 8  // Кнопка 2 (Вход)
#define BUT_2 9  // Конпка 3 (Вход)
#define BUT_1 10 // Кнопка 4 (Вход)

#define RES_4 16 // Кнопка сброса 4  (Вход)
#define RES_3 17 // Кнопка сброса 3  (Вход)
#define RES_2 23 // Кнопка сброса 2  (Вход)
#define RES_1 22 // Кнопка сброса 1  (Вход)

#define REVERS 5  // реле реверс     (Выход)
#define R_1ACT 15 // реле 1 актуатор (Выход)
#define R_2ACT 16 // реле 2 актуатор (Выход)
#define R_3ACT 1  // реле 3 актуатор (Выход)
#define R_4ACT 4  // реле 4 актуатор (Выход)

#define S_TRAY 15   // Датчик в лотке выдачи   (вход)
#define S_PROD_1 14 // Датчик 1 наличия товара (вход)
#define S_PROD_2 12 // Датчик 2 наличия товара (вход)
#define S_PROD_3 13 // Датчик 3 наличия товара (вход)
#define S_PROD_4 11 // Датчик 4 наличия товара (вход)

#define S_VID_ACT_1 7 // (Вход) Датчик концевого положения 1 актуатор (выдвинут)
#define S_ZAD_ACT_1 6 // (Вход) Датчик концевого положения 1 актуатор (задвинут)

#define S_VID_ACT_2 5 // (Вход) Датчик концевого положения 2 актуатор (выдвинут)
#define S_ZAD_ACT_2 4 // (Вход) Датчик концевого положения 2 актуатор (задвинут)

#define S_VID_ACT_3 3 // (Вход) Датчик концевого положения 3 актуатор (выдвинут)
#define S_ZAD_ACT_3 2 // (Вход) Датчик концевого положения 3 актуатор (задвинут)

#define S_VID_ACT_4 1 // (Вход) Датчик концевого положения 4 актуатор (выдвинут)
#define S_ZAD_ACT_4 0 // (Вход) Датчик концевого положения 4 актуатор (задвинут)

#define R_1 0 // Красный светодиод 1-й кнопки
#define G_1 2 // Зелёный светодиод 1-й кнопки
#define B_1 3 // Синий   светодиод 1-й кнопки

#define R_2 12 // Красный светодиод 2-й кнопки
#define G_2 13 // Зелёный светодиод 2-й кнопки
#define B_2 14 // Синий   светодиод 2-й кнопки

#define R_3 30 // Красный светодиод 3-й кнопки
#define G_3 21 // Зелёный светодиод 3-й кнопки
#define B_3 22 // Синий   светодиод 3-й кнопки

#define R_4 23 // Красный светодиод 4-й кнопки
#define G_4 24 // Зелёный светодиод 4-й кнопки
#define B_4 25 // Синий   светодиод 4-й кнопки

#define RES_TER 6      // Выход на  реле перезагрузки терминала
#define AVAIL_POWER 11 // Вход контроля наличия питания

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD -12
#define SUCCESS 0

#define F_POWER 0    // Флаг наличия питания
#define F_SERIAL 1   // Флаг отсутствия COM порта
#define F_TRAY 2     // Флаг запрета работы аппарата при несработке датчика в лотке выдачи
#define F_SERVER 3   // Флаг отсутствия связи с сервером
#define F_TERMIN 4   // Флаг отсутствия связи с терминалом
#define F_ACT1V 5    // Флаг ДКП 1 актуатор (выдвинут)
#define F_ACT1Z 6    // Флаг ДКП 1 актуатор (задвинут)
#define F_ACT2V 7    // Флаг ДКП 2 актуатор (выдвинут)
#define F_ACT2Z 8    // Флаг ДКП 2 актуатор (задвинут)
#define F_ACT3V 9    // Флаг ДКП 3 актуатор (выдвинут)
#define F_ACT3Z 10   // Флаг ДКП 3 актуатор (задвинут)
#define F_ACT4V 11   // Флаг ДКП 4 актуатор (выдвинут)
#define F_ACT4Z 12   // Флаг ДКП 4 актуатор (задвинут)
#define F_AVAILP1 13 // Флаг неисправности датчика наличия продукта 1 (ошибка датчика наличия продукта 1)
#define F_AVAILP2 14 // Флаг неисправности датчика наличия продукта 2 (ошибка датчика наличия продукта 2)
#define F_AVAILP3 15 // Флаг неисправности датчика наличия продукта 3 (ошибка датчика наличия продукта 3)
#define F_AVAILP4 16 // Флаг неисправности датчика наличия продукта 4 (ошибка датчика наличия продукта 4)

#define DW(x, y) digitalWrite(x, y)
//---------------------------------------------------------------------
void LowRgb(int num_button);                        // Выключение светодиодов кнопки
void del(int idel);                                 // Задержка в секундах
void InOutInit();                                   // Инициализация входов выходов Orange Pi
void RGB(int color, int num_button);                // Включение светодиода кнопки (кнопок)
void Actuator(int num_act, int Position);           // Управление актуатором
int myDigRead(int DSR[24]);                         // Чтение данных в сдвиговый регистр
int ReadMysqlTable(char *CommandMes, char *OutMes); // Чтение из БД в строку
int InsertMysqlTable(char *mes);                    // Отправка запроса на запись в БД
void sms(char *message);                            // Sms
int r_uart(int fd);                                 // Read from uart
char *ERRTIME(char *s1);                            // Запись времени возникновения и самой ошибки в строку для последующей записи в смс
void WriteErrMes(char *mes, int flag);              // Запись ошибок в БД
void ReadAvailProd();                               // Чтение доступных продуктов из БД
void WriteAvailProd();                              // Запись текущих продуктов в БД
int TryConnection(int flag);
char *translit(char *instr); // Функция транслитерации строки из русских символов
int burn(void);              // Функция получения прошивки по FTP

struct sockaddr_in enterSocketAddress(char *ip, char *port);
static int F_MISSING = 0;         // Флаг отсутствия товара
static int F_SELLING = 0;         // Флаг продажи
static int F_ACTUATOR = 0;        // Флаг неисправного актуатора
static int F_SW_OFF_T = 0;        // Флаг для перезагрузки терминала (если флаг=1 перезагрузку запрещаем)
static int F_SENS_AVAIL_PROD = 1; // Флаг проверки датчиков наличия товара

static uint8_t COUNT_OF_ERROR_ACT = 0; // Если ДКП актуатора стал неисправен в момент продажи и датчик лотка не сработал то увеличиваем эту переменную

static int NUM_BUT = 0;             // Номер кнопки
static int DSR[24];                 // Массив для записи значений из сдвигового регистра (Digital Shift Register)
static char receipt[1024] = {0};    // Банковская квитанция
static char ArrForCopy[1024] = {0}; // Массив "обменник" данных
static int count_1 = 0;             // Переменная счетчик
static int count_2 = 0;             // Переменная счетчик

int ArrOfErr[17] = {0}; // Для хранения всех флагов ошибок
int AvailProd[5] = {0}; // Для хранения доступных продуктов

pthread_t receive_thread, send_thread; // Для "поточных" функций отправки и приема сообщений через сокеты
static int socket1;
static int flag_message = 0;  // Флаг для отправки сообщений на сервер
static int F_CLIENT = 1;      // Флаг для (пере)подключения к серверу через сокет
static int F_CLIENT_SEND = 1; // Флаг завершения поточной функции отправки сообщений при обрыве связи с сервером

struct FtpFile // Для функции копирования с ftp
{
  const char *filename;
  FILE *stream;
};

//*****************************************************************************
// Функция кодирования ошибок в число
//*****************************************************************************
void CodeNumber(char *str)
{
  unsigned int CodeErr; // Для записи ошибок в это число (каждая ошибка кодируется соответствующим битом)
  CodeErr = 0;
  for (int i = 0; i < 17; i++)
    if (ArrOfErr[i])
      CodeErr |= (1 << i);
  sprintf(str, "ERR_%d", CodeErr);
}
//*****************************************************************************
// запись ошибок в БД и отправка смс
//*****************************************************************************
void WriteErrMes(char *mes, int flag)
{
  char ins_err[] = "INSERT INTO STATE_APP_ERR(F_POWER,F_SERIAL,F_TRAY,F_SERVER,F_TERMN,F_ACT1V,F_ACT1Z,F_ACT2V,F_ACT2Z,F_ACT3V,F_ACT3Z,F_ACT4V,F_ACT4Z,F_AVAILP1,F_AVAILP2,F_AVAILP3,F_AVAILP4,DATE_TIME)  VALUES(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,NOW())";
  char inf_app[] = "SELECT * FROM INF_APP ORDER BY ID DESC LIMIT 1";
  char str[1024];
  memset(str, 0, strlen(str));
  strcpy(str, ins_err);
  if (!flag)
  {
    for (int i = 0; i < 17; i++)
    {
      if (ArrOfErr[i])
      {
        str[190 + 2 * i] = '1';
      }
      else
        str[190 + 2 * i] = '0';
    }
    InsertMysqlTable(str); // запись ошибок в БД
  }
  ReadMysqlTable(inf_app, ArrForCopy); // информация об аппарате (номер адрес)
  memmove(&ArrForCopy[0], &ArrForCopy[2], strlen(ArrForCopy));
  memmove(&ArrForCopy[strlen(ArrForCopy)], &mes[0], strlen(mes));
  // printf("%s\n",ERRTIME(ArrForCopy));
  sms(ERRTIME(ArrForCopy)); // смс
}
//*****************************************************************************
// запись доступных продуктов в БД
//*****************************************************************************
void WriteAvailProd()
{
  char str[1024];
  char mes[] = "INSERT INTO PRODUCT(ValProd1,ValProd2,ValProd3,ValProd4,DateTime) VALUES(%d,%d,%d,%d,NOW())";
  memset(str, 0, strlen(str));
  sprintf(str, mes, AvailProd[1], AvailProd[2], AvailProd[3], AvailProd[4]);
  InsertMysqlTable(str); // запись продуктов в БД
}

//*****************************************************************************
// Чтение доступных продуктов из БД
//*****************************************************************************
void ReadAvailProd()
{
  char str[1024] = {0};
  char prod[] = "SELECT * FROM PRODUCT ORDER BY ID DESC LIMIT 1";
  int i, j, k;                           // счетчики
  if (!ReadMysqlTable(prod, ArrForCopy)) // Читаем наличие продуктов из БД
  {
    k = 0;
    j = 0;
    for (i = 0; i < strlen(ArrForCopy); i++)
    {
      if (ArrForCopy[i] != ' ')
      {
        str[k++] = ArrForCopy[i];
      }
      else
      {
        AvailProd[j++] = atoi(str);
        k = 0;
        memset(str, 0, 1024);
        if (j > 4)
          break;
      }
    }
  }
  else
  {
    sms(ERRTIME("Cannot read database"));
    exit;
  }
}
//*****************************************************************************
// Чтение значений флагов ошибок из БД
//*****************************************************************************
void ReadFlagErr()
{
  int j;
  char err[] = "SELECT * FROM STATE_APP_ERR ORDER BY ID DESC LIMIT 1";
  if (!ReadMysqlTable(err, ArrForCopy))
  {
    j = 0;
    while (ArrForCopy[j] != ' ' || j == strlen(ArrForCopy))
      j++;
    for (int i = 0; i < 17; i++)
    {
      ArrOfErr[i] = ArrForCopy[++j] - 48;
      j++;
    } // Читаем значения флагов ошибок при запуске
  }
  else
  {
    sms(ERRTIME("Cannot read database"));
    exit;
  }
}
//******************************************************************************
// извлечение информации из БД в виде строки
//******************************************************************************
int ReadMysqlTable(char *CommandMes, char *OutMes)
{
  MYSQL *conn_ptr;
  MYSQL_RES *res_ptr;
  MYSQL_ROW sqlrow;
  unsigned int timeout = 7; // Тайм-аут 7 секунд
  memset(OutMes, 0, strlen(OutMes));
  int ret = 0;
  conn_ptr = mysql_init(NULL); // инициализация
  if (!conn_ptr)
  {
    printf("mysql_init failed!\n");
    return -1;
  }

  ret = mysql_options(conn_ptr, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&timeout); // Устанавливаем параметры тайм-аута
  if (ret)
  {
    printf("Options Set ERRO!\n");
  }
  conn_ptr = mysql_real_connect(conn_ptr, "localhost", "pma", "qweasdzxc", "VENDING", 0, NULL, 0); // Подключение к базе данных MySQL
  if (conn_ptr)
  {
    printf("Connection Succeed!\n");

    ret = mysql_query(conn_ptr, CommandMes); // Выполнение оператора SQL для вывода строки
    if (!ret)
    {
      res_ptr = mysql_use_result(conn_ptr);
      if (res_ptr)
      {
        printf("Retrieved %lu rows\n", (unsigned long)mysql_num_rows(res_ptr)); // Возвращаем количество строк в наборе результатов
        while ((sqlrow = mysql_fetch_row(res_ptr)))                             // Возвращаем структуру, полученную в store_result, и получаем из нее одну строку
        {
          printf("Fetched data\n");
          unsigned int field_count = 0;
          char str[1024];
          while (field_count < mysql_field_count(conn_ptr))
          {
            sprintf(str, "%s ", sqlrow[field_count]);
            memmove(&OutMes[strlen(OutMes)], &str[0], strlen(str));
            memset(str, 0, 1024);
            field_count++;
          }
        }
      }

      if (mysql_errno(conn_ptr))
      {
        printf("Connect Erro:%d %s\n", mysql_errno(conn_ptr), mysql_error(conn_ptr)); // Возвращаем код ошибки, сообщение об ошибке
        return -2;
      }

      mysql_free_result(res_ptr);
    }
    else
    {
      printf("Connect Erro:%d %s\n", mysql_errno(conn_ptr), mysql_error(conn_ptr)); // Возвращаем код ошибки, сообщение об ошибке
      return -3;
    }

    mysql_close(conn_ptr);
    printf("Connection closed!\n");
  }
  else // Обработка ошибок
  {
    printf("Connection Failed!\n");
    if (mysql_errno(conn_ptr))
    {
      printf("Connect Erro:%d %s\n", mysql_errno(conn_ptr), mysql_error(conn_ptr)); // Возвращаем код ошибки, сообщение об ошибке
    }
    return -2;
  }

  return 0;
}

//*****************************************************************************
// Запись сообщений в БД
//*****************************************************************************
int InsertMysqlTable(char *mes)
{
  MYSQL *conn_ptr;
  MYSQL_RES *res_ptr;
  MYSQL_ROW sqlrow;
  unsigned int timeout = 7; // Тайм-аут 7 секунд

  int ret = 0;
  conn_ptr = mysql_init(NULL); // инициализация
  if (!conn_ptr)
  {
    printf("mysql_init failed!\n");
    return -1;
  }

  ret = mysql_options(conn_ptr, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&timeout); // Устанавливаем параметры тайм-аута
  if (ret)
  {
    printf("Options Set ERRO!\n");
  }

  conn_ptr = mysql_real_connect(conn_ptr, "localhost", "pma", "qweasdzxc", "VENDING", 0, NULL, 0); // Подключение к базе данных MySQL testdb
  if (conn_ptr)
  {
    printf("Connection Succeed!\n");

    {
      ret = mysql_query(conn_ptr, mes); // Выполнить инструкцию SQL

      if (!ret)
      {
        printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(conn_ptr)); // Возвращаем количество строк, измененных за последнее ОБНОВЛЕНИЕ
      }
      else
      {
        printf("Connect Erro:%d %s\n", mysql_errno(conn_ptr), mysql_error(conn_ptr)); // Возвращаем код ошибки, сообщение об ошибке
      }
    }
    mysql_close(conn_ptr);
    printf("Connection closed!\n");
  }
  else // Обработка ошибок
  {
    printf("Connection Failed!\n");
    if (mysql_errno(conn_ptr))
    {
      printf("Connect Erro:%d %s\n", mysql_errno(conn_ptr), mysql_error(conn_ptr)); // Возвращаем код ошибки, сообщение об ошибке
    }
    return -2;
  }
  return 0;
}

/************************************************************************/
// Инициализация портов gpio Orange PI
/************************************************************************/
void InOutInit()
{
  setenv("WIRINGPI_GPIOMEM", "1", 1);
  wiringPiSetup();
  pinMode(AVAIL_POWER, INPUT); // Вход контроля наличия питания
  pinMode(RES_TER, OUTPUT);    // Выход на  реле перезагрузки терминала
  pinMode(DATA, INPUT);        // Данные на вход gpio из сдвигового регистра 74hc165
  pinMode(LATCH, OUTPUT);      //"Защелка" сдвигового регистра 74hc165
  pinMode(CLOCK, OUTPUT);      // Тактовый сигнал сдвигового регистра 74hc165
  pinMode(REVERS, OUTPUT);     // РЕВЕРС
  pinMode(R_1ACT, OUTPUT);     // АКТ1 РЕЛЕ
  pinMode(R_2ACT, OUTPUT);     // АКТ2 РЕЛЕ
  pinMode(R_3ACT, OUTPUT);     // АКТ3 РЕЛЕ
  pinMode(R_4ACT, OUTPUT);     // АКТ4 РЕЛЕ
  pinMode(R_1, OUTPUT);        // Красный светодиод 1-й кнопки
  pinMode(G_1, OUTPUT);        // Зелёный светодиод 1-й кнопки
  pinMode(B_1, OUTPUT);        // Синий   светодиод 1-й кнопки
  pinMode(R_2, OUTPUT);        // Красный светодиод 2-й кнопки
  pinMode(G_2, OUTPUT);        // Зелёный светодиод 2-й кнопки
  pinMode(B_2, OUTPUT);        // Синий   светодиод 2-й кнопки
  pinMode(R_3, OUTPUT);        // Красный светодиод 3-й кнопки
  pinMode(G_3, OUTPUT);        // Зелёный светодиод 3-й кнопки
  pinMode(B_3, OUTPUT);        // Синий   светодиод 3-й кнопки
  pinMode(R_4, OUTPUT);        // Красный светодиод 4-й кнопки
  pinMode(G_4, OUTPUT);        // Зелёный светодиод 4-й кнопки
  pinMode(B_4, OUTPUT);        // Синий   светодиод 4-й кнопки
  pinMode(RES_TER, OUTPUT);    // Реле перезагрузки терминала
  pinMode(ELEC_LOCK, OUTPUT);  //(Выход) Электромагнитный замок приемного лотка
  //-------------------------------------------------------------
  DW(RES_TER, LOW);
  DW(LATCH, LOW);
  DW(CLOCK, LOW);
  DW(ELEC_LOCK, LOW);
  RGB(WHITE, ALL);
}

/************************************************************************/
// Управление актуатором
/************************************************************************/
void Actuator(int n_act, int pos)
{
  int8_t CountTime;    // Cчетчик времени   работы актуатора
  int sens = 0;        // Датчик  концевого положения актуатора
  int numact = 0;      // Для конвертирования номера актуатора
  char str[140] = {0}; // Для записи ошибки актуатора

  if ((n_act == 1) || (n_act == 2) || (n_act == 3) || (n_act == 4))
  {
    switch (n_act)
    {
    case 1:
    {
      numact = R_1ACT;
      if (pos)
      {
        sens = S_VID_ACT_1;
      }
      else
      {
        sens = S_ZAD_ACT_1;
      }
      break;
    }
    case 2:
    {
      numact = R_2ACT;
      if (pos)
      {
        sens = S_VID_ACT_2;
      }
      else
      {
        sens = S_ZAD_ACT_2;
      }
      break;
    }
    case 3:
    {
      numact = R_3ACT;
      if (pos)
      {
        sens = S_VID_ACT_3;
      }
      else
      {
        sens = S_ZAD_ACT_3;
      }
      break;
    }
    case 4:
    {
      numact = R_4ACT;
      if (pos)
      {
        sens = S_VID_ACT_4;
      }
      else
      {
        sens = S_ZAD_ACT_4;
      }
      break;
    }
    }

    if (pos)
    {
      DW(REVERS, LOW);
      DW(numact, HIGH);
    } // Если выдвинуть
    if (pos == LOW)
    {
      DW(REVERS, HIGH);
      DW(numact, LOW);
    } // Если задвинуть
    if (ArrOfErr[F_POWER] == 0)
      CountTime = 25;
    else
      CountTime = 50; // Питание от батареи или от БП
    while (!DSR[sens] && CountTime)
    {
      delay(200);
      CountTime--;
    }
    if (CountTime == 0) //  помечаем актуатор и пишем в смс и в Mysql
    {
      if (pos)
      {
        ArrOfErr[3 + (n_act) * 2] = 1;
        sprintf(str, "%s %d", " Error sensor position advanced act ", n_act);
        WriteErrMes(str, 0); // запись ошибки в смс и БД
        flag_message = 1;    // Отправка  на сервер
      }
      else
      {
        ArrOfErr[4 + (n_act) * 2] = 1;
        sprintf(str, "%s %d", " Error sensor position push act ", n_act);
        WriteErrMes(str, 0); // запись ошибки в смс и БД
        flag_message = 1;    // Отправка  на сервер
      }
    }
  }
}

/************************************************************************/
// Выключение светодиодов кнопки
/************************************************************************/
void LowRgb(int num_button)
{
  switch (num_button)
  {
  case ALL:
  {
    DW(R_1, LOW);
    DW(G_1, LOW);
    DW(B_1, LOW);
    DW(R_2, LOW);
    DW(G_2, LOW);
    DW(B_2, LOW);
    DW(R_3, LOW);
    DW(G_3, LOW);
    DW(B_3, LOW);
    DW(R_4, LOW);
    DW(G_4, LOW);
    DW(B_4, LOW);
    break;
  }
  case 1:
  {
    DW(R_1, LOW);
    DW(G_1, LOW);
    DW(B_1, LOW);
    break;
  }
  case 2:
  {
    DW(R_2, LOW);
    DW(G_2, LOW);
    DW(B_2, LOW);
    break;
  }
  case 3:
  {
    DW(R_3, LOW);
    DW(G_3, LOW);
    DW(B_3, LOW);
    break;
  }
  case 4:
  {
    DW(R_4, LOW);
    DW(G_4, LOW);
    DW(B_4, LOW);
    break;
  }
  }
}
/************************************************************************/
// Задержка в секундах
/************************************************************************/
void del(int idel)
{
  for (int i = 0; i < idel; i++)
  {
    delay(1000);
  }
}
/************************************************************************/
// Запись времени возникновения и самой ошибки в строку для последующей передачи в смс
/************************************************************************/
char *ERRTIME(char *str)
{
  char s[55];
  char tmp[120];
  struct tm *u;
  const time_t timer = time(NULL);
  u = localtime(&timer);
  memset(s, 0, 55);
  memset(tmp, 0, 120);
  strftime(s, 55, "%d.%m.%Y %H:%M:%S  ", u);
  sprintf(tmp, "%s%s", s, str);
  memset(str, 0, strlen(str));
  strcpy(str, tmp);
  return (str);
}

/************************************************************************/
// Включение светодиодов кнопки
/************************************************************************/
void RGB(int color, int num_button)
{
  if (color == WHITE)
  {
    switch (num_button)
    {
    case ALL:
    {
      DW(R_1, HIGH);
      DW(G_1, HIGH);
      DW(B_1, HIGH);
      DW(R_2, HIGH);
      DW(G_2, HIGH);
      DW(B_2, HIGH);
      DW(R_3, HIGH);
      DW(G_3, HIGH);
      DW(B_3, HIGH);
      DW(R_4, HIGH);
      DW(G_4, HIGH);
      DW(B_4, HIGH);
      break;
    }
    case 1:
    {
      DW(R_1, HIGH);
      DW(G_1, HIGH);
      DW(B_1, HIGH);
      break;
    }
    case 2:
    {
      DW(R_2, HIGH);
      DW(G_2, HIGH);
      DW(B_2, HIGH);
      break;
    }
    case 3:
    {
      DW(R_3, HIGH);
      DW(G_3, HIGH);
      DW(B_3, HIGH);
      break;
    }
    case 4:
    {
      DW(R_4, HIGH);
      DW(G_4, HIGH);
      DW(B_4, HIGH);
      break;
    }
    }
  }
  //-------------------------------------------------------------
  if (color == RED)
  {
    switch (num_button)
    {
    case ALL:
    {
      DW(R_1, HIGH);
      DW(G_1, LOW);
      DW(B_1, LOW);
      DW(R_2, HIGH);
      DW(G_2, LOW);
      DW(B_2, LOW);
      DW(R_3, HIGH);
      DW(G_3, LOW);
      DW(B_3, LOW);
      DW(R_4, HIGH);
      DW(G_4, LOW);
      DW(B_4, LOW);
      break;
    }
    case 1:
    {
      DW(R_1, HIGH);
      DW(G_1, LOW);
      DW(B_1, LOW);
      break;
    }
    case 2:
    {
      DW(R_2, HIGH);
      DW(G_2, LOW);
      DW(B_2, LOW);
      break;
    }
    case 3:
    {
      DW(R_3, HIGH);
      DW(G_3, LOW);
      DW(B_3, LOW);
      break;
    }
    case 4:
    {
      DW(R_4, HIGH);
      DW(G_4, LOW);
      DW(B_4, LOW);
      break;
    }
    }
  }
  //-------------------------------------------------------------
  if (color == GREEN)
  {
    switch (num_button)
    {
    case ALL:
    {
      DW(R_1, LOW);
      DW(G_1, HIGH);
      DW(B_1, LOW);
      DW(R_2, LOW);
      DW(G_2, HIGH);
      DW(B_2, LOW);
      DW(R_3, LOW);
      DW(G_3, HIGH);
      DW(B_3, LOW);
      DW(R_4, LOW);
      DW(G_4, HIGH);
      DW(B_4, LOW);
      break;
    }
    case 1:
    {
      DW(R_1, LOW);
      DW(G_1, HIGH);
      DW(B_1, LOW);
      break;
    }
    case 2:
    {
      DW(R_2, LOW);
      DW(G_2, HIGH);
      DW(B_2, LOW);
      break;
    }
    case 3:
    {
      DW(R_3, LOW);
      DW(G_3, HIGH);
      DW(B_3, LOW);
      break;
    }
    case 4:
    {
      DW(R_4, LOW);
      DW(G_4, HIGH);
      DW(B_4, LOW);
      break;
    }
    }
  }
  //-------------------------------------------------------------
  if (color == BLUE)
  {
    switch (num_button)
    {
    case ALL:
    {
      DW(R_1, LOW);
      DW(G_1, LOW);
      DW(B_1, HIGH);
      DW(R_2, LOW);
      DW(G_2, LOW);
      DW(B_2, HIGH);
      DW(R_3, LOW);
      DW(G_3, LOW);
      DW(B_3, HIGH);
      DW(R_4, LOW);
      DW(G_4, LOW);
      DW(B_4, HIGH);
      break;
    }
    case 1:
    {
      DW(R_1, LOW);
      DW(G_1, LOW);
      DW(B_1, HIGH);
      break;
    }
    case 2:
    {
      DW(R_2, LOW);
      DW(G_2, LOW);
      DW(B_2, HIGH);
      break;
    }
    case 3:
    {
      DW(R_3, LOW);
      DW(G_3, LOW);
      DW(B_3, HIGH);
      break;
    }
    case 4:
    {
      DW(R_4, LOW);
      DW(G_4, LOW);
      DW(B_4, HIGH);
      break;
    }
    }
  }
  //-------------------------------------------------------------
} // end RGB

/************************************************************************/
// Чтение данных из сдвигового регистра в массив DSR (Digital Shift Register)
/************************************************************************/
int myDigRead(int DSR[24])
{
  int INTPRV[24] = {0};
  int res;
  res = 0;
  DW(LATCH, LOW);
  delayMicroseconds(10);
  DW(LATCH, HIGH);
  for (uint8_t i = 0; i < 24; i++)
  {
    delayMicroseconds(10);
    INTPRV[i] = digitalRead(DATA);
    if (DSR[i] != INTPRV[i])
    {
      res = 1;
    }
    DSR[i] = INTPRV[i];
    DW(CLOCK, LOW);
    delayMicroseconds(10);
    DW(CLOCK, HIGH);
  }
  return res;
}
/************************************************************************/
// Отправка всего сообщения на сервер через сокет
/************************************************************************/
int sendall(int s, char *buf, int len, int flags)
{
  int total = 0;
  int n;

  while (total < len)
  {
    n = send(s, buf + total, len - total, flags);
    if (n == -1)
    {
      break;
    }
    total += n;
  }

  return (n == -1 ? -1 : total);
}
/************************************************************************/
// Read from Uart
/************************************************************************/
int r_uart(int fd)
{
  char data[20];
  int i = 0;
  while (serialDataAvail(fd))
  {
    data[i] = serialGetchar(fd);
    printf("%c", data[i]);
    i++;
    fflush(stdout);
  }
  if (i)
    printf("\n");
  return i;
}

/************************************************************************/
// Отправка смс
/************************************************************************/
void sms(char *message)
{
  int i = 0;
  int j = 2;
  int fd; // Дескриптор ком порта модема
  char Buf[13];
  char numbers[1024];
  char GiveMeTelNumber[] = "SELECT * FROM NUM_TEL ORDER BY ID";
  //  printf("%s%s\n","AT+CMGS=","+79529174466");

  if ((fd = serialOpen("/dev/MyDevices/Modem", 115200)) < 0) //
  {
    fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
  }

  if (!ReadMysqlTable(GiveMeTelNumber, numbers)) // если открыта бд с номерами телефонов
  {
    while (j < strlen(numbers))
    {
      memset(Buf, 0, 13);
      memmove(&Buf[0], &numbers[j], 12);
      j = j + 15;
      Buf[12] = 0;
      printf("%d %d ", strlen(numbers), j);
      serialPuts(fd, "AT+CMGD=1,4\r");
      printf("%s\n", "AT+CMGD=1,4\r");
      while (!r_uart(fd))
      {
        delay(100);
        i++;
        if (i > 50)
        {
          printf("Modem not answer\n");
          i = 0;
          break;
        }
      };
      i = 0;
      serialPuts(fd, "AT+CSCS=\"GSM\"\r");
      printf("%s\n", "AT+CSCS=\"GSM\"\r");
      while (!r_uart(fd))
      {
        delay(100);
        i++;
        if (i > 50)
        {
          printf("Modem not answer\n");
          i = 0;
          break;
        }
      };
      i = 0;
      serialPuts(fd, "AT+CMGF=1\r");
      printf("%s\n", "AT+CMGF=1\r");
      while (!r_uart(fd))
      {
        delay(100);
        i++;
        if (i > 50)
        {
          printf("Modem not answer\n");
          i = 0;
          break;
        }
      };
      i = 0;
      serialPuts(fd, "AT+CMGS=");
      serialPuts(fd, "\"");
      serialPuts(fd, Buf);
      serialPuts(fd, "\"");
      serialPuts(fd, "\r");
      printf("%s%s\n", "AT+CMGS=", Buf);
      while (!r_uart(fd))
      {
        delay(100);
        i++;
        if (i > 50)
        {
          printf("Modem not answer\n");
          i = 0;
          break;
        }
      };
      i = 0;
      serialPuts(fd, message);
      printf("%s\n", message);
      delay(1500);
      serialPutchar(fd, 0x1A);
      printf("%s\n", "0x1A");
      while (!r_uart(fd))
      {
        delay(100);
        i++;
        if (i > 50)
        {
          printf("Modem not answer\n");
          i = 0;
          break;
        }
      };
      i = 0;
    }
  }

  else // Иначе шлем смс на номер по умолчанию
  {
    serialPuts(fd, "AT+CMGD=1,4\r");
    printf("%s\n", "AT+CMGD=1,4\r");
    while (!r_uart(fd))
    {
      delay(100);
      i++;
      if (i > 50)
      {
        printf("Modem not answer\n");
        i = 0;
        break;
      }
    };
    i = 0;
    serialPuts(fd, "AT+CSCS=\"GSM\"\r");
    printf("%s\n", "AT+CSCS=\"GSM\"\r");
    while (!r_uart(fd))
    {
      delay(100);
      i++;
      if (i > 50)
      {
        printf("Modem not answer\n");
        i = 0;
        break;
      }
    };
    i = 0;
    serialPuts(fd, "AT+CMGF=1\r");
    printf("%s\n", "AT+CMGF=1\r");
    while (!r_uart(fd))
    {
      delay(100);
      i++;
      if (i > 50)
      {
        printf("Modem not answer\n");
        i = 0;
        break;
      }
    };
    i = 0;
    serialPuts(fd, "AT+CMGS=");
    serialPuts(fd, "\"");
    serialPuts(fd, "+79529174466");
    serialPuts(fd, "\"");
    serialPuts(fd, "\r");
    printf("%s%s\n", "AT+CMGS=", "+79529174466");
    while (!r_uart(fd))
    {
      delay(100);
      i++;
      if (i > 50)
      {
        printf("Modem not answer\n");
        i = 0;
        break;
      }
    };
    i = 0;
    serialPuts(fd, message);
    printf("%s\n", message);
    delay(1500);
    serialPutchar(fd, 0x1A);
    printf("%s\n", "0x1A");
    while (!r_uart(fd))
    {
      delay(100);
      i++;
      if (i > 50)
      {
        printf("Modem not answer\n");
        i = 0;
        break;
      }
    };
    i = 0;
  }

  serialClose(fd);
}

/************************************************************************/
// Связь с терминалом
/************************************************************************/
int TryConnection(int flag)
{
  int i, fd;

  if ((fd = serialOpen("/dev/MyDevices/Terminal", 115200)) < 0) //
  {
    if (ArrOfErr[F_SERIAL] == 0)
    {
      fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
      ArrOfErr[F_SERIAL] = 1;                    // Взводим флаг ошибки com порта
      flag_message = 1;                          // Шлем на сервер
      WriteErrMes("Terminal com port error", 0); // пишем ошибку в БД отсылаем смс
    }
    return fd;
  }
  else
  {
    if (ArrOfErr[F_SERIAL] == 1)
    {
      ArrOfErr[F_SERIAL] = 0;
      flag_message = 1;                       // Шлем на сервер
      WriteErrMes("Terminal com port ok", 0); // пишем ошибку в БД отсылаем смс
    }

    if (flag == 1)
    {
      i = 0;
      gen_IDL(&req);
      sendUART(&req, fd);
      delay(100);
      while (!read_from_UART(&req, fd))
      {
        delay(100);
        i++;
        if (i > 70)
        {
          i = 0;
          count_1++;
          printf("Terminal not answer \n");
          break;
        }
      }

      if (count_1 == 3) // Если терминал не ответил 3 раза
      {
        ArrOfErr[F_TERMIN] = 1;
        flag_message = 1;
        WriteErrMes("Terminal not answer", 0); // отсылаем смс пишем ошибку в БД
      }

      if (i > 0)
      {
        i = 0;
        // если связь восстановлена после ошибки счетчик в 0 смс и в БД пишем
        if (ArrOfErr[F_TERMIN])
        {
          count_1 = 0;
          count_2 = 0;
          ArrOfErr[F_TERMIN] = 0;
          flag_message = 1;
          WriteErrMes("Terminal ok", 0);
        }
        else
        {
          count_1 = 0;
          count_2 = 0;
        }
      }
      return fd;
    } // end if (flag==1)

    if (flag == 2)
    {
      i = 0;
      gen_DIS(&req);
      sendUART(&req, fd);
      delay(100);
      while (!read_from_UART(&req, fd))
      {
        delay(100);
        i++;
        if (i > 50)
        {
          i = 0;
          count_2++;
          printf("Terminal not answer \n");
          break;
        }
      }

      if (count_2 == 35) // Если терминал не ответил 35 раза
      {
        ArrOfErr[F_TERMIN] = 1;
        flag_message = 1;                      // шлем на сервер
        WriteErrMes("Terminal not answer", 0); // отсылаем смс пишем ошибку в БД
      }

      if (i > 0)
      {
        i = 0;
        // если связь восстановлена после ошибки счетчик в 0 смс и файл пишем
        if (ArrOfErr[F_TERMIN])
        {
          count_1 = 0;
          count_2 = 0;
          ArrOfErr[F_TERMIN] = 0;
          flag_message = 1; // шлем на сервер
          WriteErrMes("Terminal ok", 0);
        }
        else
        {
          count_1 = 0;
          count_2 = 0;
        }
        del(5);
      }
      return fd;
    } // end if (flag==2)

    if (flag == 3)
    {
      i = 0;
      gen_IDL_SCREEN_EMPTY(&req);
      sendUART(&req, fd);
      delay(100);
      while (!read_from_UART(&req, fd))
      {
        delay(100);
        i++;
        if (i > 30)
        {
          i = 0;
          printf("Terminal not answer \n");
          break;
        }
      }
      return fd;
    } // end if (flag==3)

    if (flag == 4)
    {
      i = 0;
      gen_IDL_SCREEN_OWNER(&req);
      sendUART(&req, fd);
      while (!read_from_UART(&req, fd))
      {
        delay(100);
        i++;
        if (i > 30)
        {
          i = 0;
          printf("Terminal not answer \n");
          break;
        }
      }
      return fd;
    } // end if (flag==4)

    if (flag == 5)
    {
      i = 0;
      gen_IDL(&req);
      sendUART(&req, fd);
      while (!read_from_UART(&req, fd))
      {
        delay(100);
        i++;
        if (i > 30)
        {
          i = 0;
          printf("Terminal not answer \n");
          break;
        }
      }
      return fd;
    } // end if (flag==5)
  }
}

/************************************************************************/
// Функция транслитерации строки
/************************************************************************/
char *translit(char *instr)
{
  int i = 0, j = 0;
  char outstr[1024];
  memset(outstr, 0, strlen(outstr));
  while (i < strlen(instr))
  {
    if (instr[i] == 0xd0 && instr[i + 1] == 0x90)
    {
      outstr[j++] = 'A';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x91)
    {
      outstr[j++] = 'B';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x92)
    {
      outstr[j++] = 'V';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x93)
    {
      outstr[j++] = 'G';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x94)
    {
      outstr[j++] = 'D';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x95)
    {
      outstr[j++] = 'E';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x96)
    {
      outstr[j++] = 'J';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x97)
    {
      outstr[j++] = 'Z';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x98)
    {
      outstr[j++] = 'I';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x99)
    {
      outstr[j++] = 'Y';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x9A)
    {
      outstr[j++] = 'K';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x9B)
    {
      outstr[j++] = 'L';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x9C)
    {
      outstr[j++] = 'M';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x9D)
    {
      outstr[j++] = 'N';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x9E)
    {
      outstr[j++] = 'O';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x9F)
    {
      outstr[j++] = 'P';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xA0)
    {
      outstr[j++] = 'R';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xA1)
    {
      outstr[j++] = 'S';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xA2)
    {
      outstr[j++] = 'T';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xA3)
    {
      outstr[j++] = 'U';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xA4)
    {
      outstr[j++] = 'F';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xA5)
    {
      outstr[j++] = 'X';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xA6)
    {
      outstr[j++] = 'C';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xA7)
    {
      outstr[j++] = 'C';
      outstr[j++] = 'H';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xA8)
    {
      outstr[j++] = 'S';
      outstr[j++] = 'H';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xA9)
    {
      outstr[j++] = 'S';
      outstr[j++] = 'H';
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xAA)
    {
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xAB)
    {
      outstr[j++] = 'Y';
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xAC)
    {
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xAD)
    {
      outstr[j++] = 'E';
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xAE)
    {
      outstr[j++] = 'Y';
      outstr[j++] = 'U';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xAF)
    {
      outstr[j++] = 'Y';
      outstr[j++] = 'A';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xB0)
    {
      outstr[j++] = 'a';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xB1)
    {
      outstr[j++] = 'b';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xB2)
    {
      outstr[j++] = 'v';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xB3)
    {
      outstr[j++] = 'g';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xB4)
    {
      outstr[j++] = 'd';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xB5)
    {
      outstr[j++] = 'e';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xB6)
    {
      outstr[j++] = 'j';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xB7)
    {
      outstr[j++] = 'z';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xB8)
    {
      outstr[j++] = 'i';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xB9)
    {
      outstr[j++] = 'y';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xBA)
    {
      outstr[j++] = 'k';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xBB)
    {
      outstr[j++] = 'l';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xBC)
    {
      outstr[j++] = 'm';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xBD)
    {
      outstr[j++] = 'n';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xBE)
    {
      outstr[j++] = 'o';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0xBF)
    {
      outstr[j++] = 'p';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x80)
    {
      outstr[j++] = 'r';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x81)
    {
      outstr[j++] = 's';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x82)
    {
      outstr[j++] = 't';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x83)
    {
      outstr[j++] = 'u';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x84)
    {
      outstr[j++] = 'f';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x85)
    {
      outstr[j++] = 'x';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x86)
    {
      outstr[j++] = 'c';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x87)
    {
      outstr[j++] = 'c';
      outstr[j++] = 'h';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x88)
    {
      outstr[j++] = 's';
      outstr[j++] = 'h';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x89)
    {
      outstr[j++] = 's';
      outstr[j++] = 'h';
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x8A)
    {
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x8B)
    {
      outstr[j++] = 'y';
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x8C)
    {
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x8D)
    {
      outstr[j++] = 'e';
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x8E)
    {
      outstr[j++] = 'y';
      outstr[j++] = 'u';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x8F)
    {
      outstr[j++] = 'y';
      outstr[j++] = 'a';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd0 && instr[i + 1] == 0x81)
    {
      outstr[j++] = 'E';
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    if (instr[i] == 0xd1 && instr[i + 1] == 0x91)
    {
      outstr[j++] = 'e';
      outstr[j++] = '`';
      i += 2;
      continue;
    }
    outstr[j++] = instr[i++];
  } // end while
  memset(instr, 0, strlen(instr));
  sprintf(instr, "%s", outstr);
  return instr;
} // end func translit

#endif /*MAIN_H_ */
