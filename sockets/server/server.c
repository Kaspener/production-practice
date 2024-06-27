#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/reboot.h>
#include "socketFunctions.h"
#include <mysql/mysql.h>

struct ThreadData *clients[MAX_CLIENTS];
int countOfClients = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
  conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root", "nbvhgfytr", "vending", 0, NULL, 0); // Подключение к базе данных MySQL
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
            snprintf(str, 1024, "%s ", sqlrow[field_count]);
            memmove(&OutMes[strlen(OutMes)], &str[0], strlen(str));
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

  conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root", "nbvhgfytr", "vending", 0, NULL, 0); // Подключение к базе данных MySQL testdb
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

void updateErrorsDatabase(int errorNum, int num)
{
  char serial[128];
  char address[128];
  char query[1024];
  bool flag = true;
  memset(serial, 0, 128);
  memset(address, 0, 128);
  memset(query, 0, 1024);
  sprintf(query, "SELECT serial FROM vending WHERE ID = %d", num);
  ReadMysqlTable(query, serial);
  sprintf(query, "SELECT address FROM vending WHERE ID = %d", num);
  ReadMysqlTable(query, address);
  static const char messages[17][128] = {
      "Внешнее электропитание отключено",
      "COM порт терминала неисправен",
      "Неисправность датчика в лотке выдачи",
      "Отсутствие связи с сервером",
      "Отсутствие связи с терминалом",
      "ДКП 1 актуатора поз выдвинут",
      "ДКП 1 актуатора поз задвинут",
      "ДКП 2 актуатора поз выдвинут",
      "ДКП 2 актуатора поз задвинут",
      "ДКП 3 актуатора поз выдвинут",
      "ДКП 3 актуатора поз задвинут",
      "ДКП 4 актуатора поз выдвинут",
      "ДКП 4 актуатора поз задвинут",
      "Неисправность датчика наличия продукта 1",
      "Неисправность датчика наличия продукта 2",
      "Неисправность датчика наличия продукта 3",
      "Неисправность датчика наличия продукта 4"};
  for (int i = 0; i < 17; ++i)
  {
    if (errorNum & (1 << i))
    {
      flag = false;
      char temp[1024];
      char result[128];
      sprintf(temp, "SELECT COUNT(*) FROM errors WHERE numberOfDevice = %d AND endError is NULL AND error='%s'", num, messages[i]);
      ReadMysqlTable(temp, result);
      if (result[0] == '0')
      {
        sprintf(query, "INSERT INTO errors (numberOfDevice, serial, address, error, startError) VALUES (%d, '%s', '%s', '%s', NOW())", num, serial, address, messages[i]);
        InsertMysqlTable(query);
      }
    }
    else
    {
      sprintf(query, "UPDATE errors SET endError=NOW() WHERE error='%s' AND numberOfDevice=%d AND endError is NULL", messages[i], num);
      InsertMysqlTable(query);
    }
  }
  if (!flag)
  {
    sprintf(query, "UPDATE vending SET deviceCondition=0 WHERE ID=%d", num);
    InsertMysqlTable(query);
  }
  else
  {
    sprintf(query, "UPDATE vending SET deviceCondition=1 WHERE ID=%d", num);
    InsertMysqlTable(query);
  }
}

void updateSell(int *AvailProd, int num)
{
  char query[1024];
  char counts[256];
  char serial[128];
  char address[128];
  char priceChar[16];
  int price;
  memset(counts, 0, 256);
  memset(serial, 0, 128);
  memset(address, 0, 128);
  memset(priceChar, 0, 16);
  sprintf(query, "SELECT 1Count, 2Count, 3Count, 4Count FROM vending WHERE ID = %d", num);
  ReadMysqlTable(query, counts);
  sprintf(query, "SELECT serial FROM vending WHERE ID = %d", num);
  ReadMysqlTable(query, serial);
  sprintf(query, "SELECT address FROM vending WHERE ID = %d", num);
  ReadMysqlTable(query, address);
  sprintf(query, "SELECT price FROM vending WHERE ID = %d", num);
  ReadMysqlTable(query, priceChar);
  price = atoi(priceChar);
  char Prod[4];
  int CurProd[4];
  int k = 0, j = 0;
  for (int i = 0; i < strlen(counts); ++i)
  {
    if (counts[i] != ' ')
    {
      Prod[k++] = counts[i];
    }
    else
    {
      CurProd[j++] = atoi(Prod);
      k = 0;
      memset(Prod, 0, 4);
      if (j > 4)
        break;
    }
  }

  int ct = 0;
  for (int i = 0; i < 4; ++i)
  {
    ct += CurProd[i] - AvailProd[i];
  }
  if (ct != 0)
  {
    sprintf(query, "UPDATE vending SET 1Count=%d, 2Count=%d, 3Count=%d, 4Count=%d, lastSellDate=NOW() WHERE ID = %d", AvailProd[0], AvailProd[1], AvailProd[2], AvailProd[3], num);
    InsertMysqlTable(query);
    sprintf(query, "INSERT INTO sellAndLoadHistory (numberOfDevice, serial, address, datatime, cell1, cell2, cell3, cell4, isSell, remains1, remains2, remains3, remains4) VALUES (%d, '%s', '%s', NOW(), %d, %d, %d, %d, 1, %d, %d, %d, %d)", num, serial, address, CurProd[0] - AvailProd[0], CurProd[1] - AvailProd[1], CurProd[2] - AvailProd[2], CurProd[3] - AvailProd[3], AvailProd[0], AvailProd[1], AvailProd[2], AvailProd[3]);
    InsertMysqlTable(query);
    sprintf(query, "INSERT INTO sales (numberOfDevice, serial, address, price, data, count, amount) VALUES (%d, '%s', '%s', %d, NOW(), %d, %d)", num, serial, address, price, ct, ct * price);
    InsertMysqlTable(query);
  }
}

/********* Wait for some number of milliseconds  */
void delay(unsigned int howLong)
{
  struct timespec sleeper, dummy;
  sleeper.tv_sec = (time_t)(howLong / 1000);
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000;
  nanosleep(&sleeper, &dummy);
}

struct ThreadData // Для хранения данных о подключенных клиентах
{
  int client_socket;
  struct sockaddr_in client_address;
  char buffer[BUFFERSIZE];
  char name[MAX_NAME_LENGTH];
};

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

void removeClientSocket(int socket)
{
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < countOfClients; ++i)
  {
    if (clients[i]->client_socket == socket)
    {
      for (int j = i + 1; j < countOfClients; ++j)
      {
        clients[j - 1] = clients[j];
      }
      countOfClients--;
      clients[countOfClients] = NULL;
      break;
    }
  }
  pthread_mutex_unlock(&mutex);
}

void broadcastMessage(char *message) //, int sender_socket
{
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < countOfClients; ++i)
  {
    // if (clients[i]->client_socket != sender_socket)
    {
      sendall(clients[i]->client_socket, message, strlen(message), 0);
    }
  }
  pthread_mutex_unlock(&mutex);
}

int privateMessage(char *message, char *name)
{
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < countOfClients; ++i)
  {
    if (strlen(clients[i]->name) == strlen(name))
    {
      if (strcmp(clients[i]->name, name) == 0)
      {
        sendall(clients[i]->client_socket, message, strlen(message), 0);
        pthread_mutex_unlock(&mutex);
      }
    }
  }
  pthread_mutex_unlock(&mutex);
  return 0;
}

void clientConnected(struct ThreadData *data)
{
  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(data->client_address.sin_addr), client_ip, INET_ADDRSTRLEN);
  int client_port = ntohs(data->client_address.sin_port);
  pthread_mutex_lock(&mutex);
  printf("[LOG] Client %s connected: %s:%d\n", data->name, client_ip, client_port);
  pthread_mutex_unlock(&mutex);
}

void clientDisconnected(struct ThreadData *data)
{
  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(data->client_address.sin_addr), client_ip, INET_ADDRSTRLEN);
  int client_port = ntohs(data->client_address.sin_port);
  pthread_mutex_lock(&mutex);
  printf("[LOG] Client %s disconnected: %s:%d\n", data->name, client_ip, client_port);
  pthread_mutex_unlock(&mutex);
  removeClientSocket(data->client_socket);
  close(data->client_socket);
}

void *clientThread(void *data)
{
  struct ThreadData *thread_data = (struct ThreadData *)data;
  int client_socket = thread_data->client_socket;
  char *buffer = thread_data->buffer;
  char query[1024];
  char *name = thread_data->name;
  char en[] = "en";
  char ok[] = "ok ";
  struct sockaddr_in client_address = thread_data->client_address;

  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(client_address.sin_addr), client_ip, INET_ADDRSTRLEN);
  int client_port = ntohs(client_address.sin_port);
  ssize_t bytes_received;
  ssize_t len;
  clientConnected(thread_data);
  while (1)
  {
    memset(buffer, 0, BUFFERSIZE);
    bytes_received = recv(client_socket, buffer, 1024, 0);
    //  recv(client_socket, buffer, 128, MSG_WAITALL);

    if (bytes_received <= 0)
    {
      clientDisconnected(thread_data);
      free(thread_data);
      pthread_exit(EXIT_SUCCESS);
    }
    else
    {

      int num = (name[0] - '0') * 100 + (name[1] - '0') * 10 + (name[2] - '0');

      if (strncmp(buffer, en, 2) == 0)
      {
        sendall(client_socket, ok, strlen(ok), 0);
        printf("%s\n", buffer);
        memset(query, 0, strlen(query));
        sprintf(query, "UPDATE vending SET lastConnectionDate=NOW() WHERE ID = %d", num);
        InsertMysqlTable(query);
      }
      else
      {

        if (strncmp(buffer, "ERR_", 4) == 0)
        {
          memmove(buffer, buffer + 4, strlen(buffer) - 3);
          int errorsNum = atoi(buffer);
          updateErrorsDatabase(errorsNum, num);
        }
        else if (strncmp(buffer, "PR_", 3) == 0)
        {
          char Prod[4];
          int AvailProd[4];
          int k = 0, j = 0;
          for (int i = 3; i < strlen(buffer); ++i)
          {
            if (buffer[i] != '_')
            {
              Prod[k++] = buffer[i];
            }
            else
            {
              AvailProd[j++] = atoi(Prod);
              k = 0;
              memset(Prod, 0, 4);
              if (j > 4)
                break;
            }
          }
          updateSell(AvailProd, num);
        }
      }
      printf("%s\n", buffer);
    }
  } // end while
}

void *send_message(void *arg)
{
  char message[1024];
  int countTime = 0;
  memset(message, 0, 1024);
  struct ThreadData *threadData = (struct ThreadData *)malloc(sizeof(struct ThreadData));
  threadData->client_socket = 0;
  memset(threadData->name, 0, MAX_NAME_LENGTH);
  memcpy(threadData->name, "SERVER", 6);
  while (1)
  {
    char query[1024];
    char result[256];
    ReadMysqlTable("SELECT COUNT(*) FROM operations", result);
    if (result[0] == '0')
    {
      printf("No command\n");
    }
    else
    {
      for (int i = 0; i < countOfClients; ++i)
      {
        char *name = clients[i]->name;
        int num = (name[0] - '0') * 100 + (name[1] - '0') * 10 + (name[2] - '0');
        char query[1024];
        sprintf(query, "SELECT COUNT(*) FROM operations WHERE numberOfDevice = %d", num);
        ReadMysqlTable(query, result);
        if (result[0] == '0')
          continue;
        else
        {
          int count = atoi(result);
          printf("%d count \n", count);
          memset(message, 0, 1024);
          memcpy(message, "/priv_", 6);
          char command[128];
          for (int j = 0; j < count; ++j)
          {
            memset(query, 0, 1024);
            sprintf(query, "SELECT command  FROM operations WHERE numberOfDevice = %d ORDER BY numberOfDevice DESC LIMIT 1", num);
            ReadMysqlTable(query, command);

            memset(query, 0, 1024);
            sprintf(query, "DELETE  FROM operations WHERE numberOfDevice = %d ORDER BY numberOfDevice DESC LIMIT 1", num);
            InsertMysqlTable(query);

            command[strlen(command) - 1] = 0;
            printf("Name = %s ; Command = %s\n", name, command);
            for (int i = 0; i < 3; ++i)
            {
              message[6 + i] = name[i];
            }
            message[9] = '_';
            for (int i = 0; i <= strlen(command); ++i)
            {
              message[10 + i] = command[i];
            }
            printf("%s\n", message);
            if (strncmp(message, "/priv", 5) != 0)
              broadcastMessage(message);
            else
            {
              char name[4] = "   \0";
              for (int i = 0; i < 3; ++i)
              {
                name[i] = message[i + 6];
              }
              memmove(message, message + 10, strlen(message) - 9);
              printf("Name = %s; Message = %s\n", name, message);
              privateMessage(message, name);
              memcpy(message, "/priv_", 6);
            }
            delay(300);
          }
        }
      }
    } // end else if command

    delay(5000);
    countTime++;
    if (countTime > 20)
    {
      countTime = 0;
      system("clear");
    }
  } // end while
} // end function

int main()
{
  struct ThreadData *threadData;

  pthread_t thread_id, send_thread;                   // Дескрипторы потоков
  int clientSocket;                                   // Дескриптор сокета клиента
  int serverSocket = Socket(AF_INET, SOCK_STREAM, 0); // Создаем сокет и присваеваем ему конкретный дескриптор
  struct sockaddr_in serverAddress, clientAddress;    // структура для хранения адреса сервера и клиента
  serverAddress.sin_family = AF_INET;                 // Семейство адресов ipv4
  serverAddress.sin_addr.s_addr = INADDR_ANY;         // соединяться с клиентами через любой сетевой интерфейс константа INADDR_ANY
  serverAddress.sin_port = htons(65150);              // порт

  socklen_t length = sizeof(serverAddress);   // Размер структуры адреса
  Bind(serverSocket, &serverAddress, length); // Связывание сокета с конкретным ip адресом и портом
  // GetSockName(serverSocket, &serverAddress, &length);
  Listen(serverSocket, LISTEN_QUEUE); // При этом сокет переводится в режим ожидания запросов
  // со стороны клиентов.
  //  LISTEN_QUEUE размер очереди  запросов

  /*
  Каждый раз, когда очередной клиент пытается соединиться с сервером,
  его запрос ставится в очередь, так как сервер может быть
  занят обработкой других запросов. Если очередь заполнена, все последующие запросы будут игнорироваться.
  Когда сервер готов обслужить очередной запрос, он использует функцию accept.
  */
  pthread_create(&send_thread, NULL, send_message, NULL); // создание потока (посылка сообщений от сервера )

  printf("Open server:\nPort = %d\n", ntohs(serverAddress.sin_port));
  while (1)
  {
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &length);
    if (clientSocket == -1)
    {
      perror("Accept failed");
      continue;
    }
    /*
     Функция accept создаёт для общения с клиентом новый сокет и возвращает его дескриптор.
     Параметр sockfd задаёт слушающий сокет. После вызова он остаётся в слушающем состоянии
     и может принимать другие соединения. В структуру, на которую ссылается addr, записывается
     адрес сокета клиента, который установил соединение с сервером. В переменную, адресуемую
     указателем addrlen, изначально записывается размер структуры; функция accept записывает
     туда длину, которая реально была использована. Если вас не интересует адрес клиента, вы можете
     просто передать NULL в качестве второго и третьего параметров.Обратите внимание, что полученный
     от accept новый сокет связан с тем же самым адресом, что и слушающий сокет.
     Сначала это может показаться странным. Но дело в том, что адрес TCP-сокета не обязан
     быть уникальным в Internet-домене. Уникальными должны быть только соединения,
     для идентификации которых используются два адреса сокетов, между которыми
     происходит обмен данными.
    */

    threadData = (struct ThreadData *)malloc(sizeof(struct ThreadData));
    threadData->client_address = clientAddress;
    threadData->client_socket = clientSocket;
    memset(threadData->buffer, 0, BUFFERSIZE);
    memset(threadData->name, 0, MAX_NAME_LENGTH);
    recv(clientSocket, threadData->name, sizeof(threadData->name), 0);

    if (countOfClients < MAX_CLIENTS)
    {
      memset(threadData->buffer, OKEY, 1);
      threadData->buffer[1] = 0;
      send(clientSocket, threadData->buffer, strlen(threadData->buffer), 0);
      clients[countOfClients++] = threadData;
    }
    else
    {
      memset(threadData->buffer, NO_PLACE, 1);
      threadData->buffer[1] = 0;
      send(clientSocket, threadData->buffer, strlen(threadData->buffer), 0);
      free(threadData);
      continue;
    }

    if (pthread_create(&thread_id, NULL, clientThread, threadData))
    {
      perror("Thread creation failed");
      close(clientSocket);
      free(threadData);
    }
    else
    {
      pthread_detach(thread_id);
      /*
       Функция pthread_detach() помечает нить, указанную thread, как отсоединённую.
       Когда отсоединённая нить завершается, её ресурсы автоматически отдаются обратно
       системе и не нужно объединять другую нить с завершившей работу.
       Попытка отсоединить уже отсоединённую нить приводит к непредсказуемым последствиям.
      */
    }
  }
  pthread_join(send_thread, NULL); // используется для ожидания завершения потока
  close(serverSocket);
  return EXIT_SUCCESS;
}
