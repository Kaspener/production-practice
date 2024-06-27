#ifndef SOCKET_FUNCTIONS_H
#define SOCKET_FUNCTIONS_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFERSIZE 1024
#define LISTEN_QUEUE 5
#define MAX_CLIENTS 100
#define MAX_NAME_LENGTH 16
#define MESSAGE_SIZE 1
#define TYPE_SIZE 1

#define BROADCASTSERVER '2'
#define PRIVATESERVER '3'
#define USERLIST '4'
#define ERRORMESSAGE '5'

#define OKEY '0'
#define NO_PLACE '1'
#define NO_NAME '2'

#define NAMEC1  '0'
#define NAMEC2  '0'
#define NAMEC3  '1'

#define MES0  "en"
#define MES1  "F_POWER"	// Флаг наличия питания (Внешнее электропитание отключено)
#define MES2  "F_SERIAL"	// Флаг отсутствия COM порта (COM порт терминала неисправен)
#define MES3  "F_TRAY"		// Флаг запрета работы аппарата при несработке датчика в лотке выдачи (Неисправность датчика в лотке выдачи)
#define MES4  "F_SERVER"	// Флаг отсутствия связи с сервером (Отсутствие связи с сервером)
#define MES5  "F_TERMIN"	// Флаг отсутствия связи с терминалом (Отсутствие связи с терминалом)
#define MES6  "F_ACT1V"	// Флаг ДКП 1 актуатора (выдвинут) (ДКП 1 актуатора поз выдвинут)
#define MES7  "F_ACT1Z"	// Флаг ДКП 1 актуатора (задвинут) (ДКП 1 актуатора поз задвинут)
#define MES8  "F_ACT2V"	// Флаг ДКП 2 актуатора (выдвинут) (ДКП 2 актуатора поз выдвинут)
#define MES9  "F_ACT2Z"	// Флаг ДКП 2 актуатора (задвинут) (ДКП 2 актуатора поз задвинут)
#define MES10 "F_ACT3V"	// Флаг ДКП 3 актуатора (выдвинут) (ДКП 3 актуатора поз выдвинут)
#define MES11 "F_ACT3Z"	// Флаг ДКП 3 актуатора (задвинут) (ДКП 3 актуатора поз задвинут)
#define MES12 "F_ACT4V"	// Флаг ДКП 4 актуатора (выдвинут) (ДКП 4 актуатора поз выдвинут)
#define MES13 "F_ACT4Z"	// Флаг ДКП 4 актуатора (задвинут) (ДКП 4 актуатора поз задвинут)
#define MES14 "F_AVAILP1"	// Флаг неисправности датчика наличия продукта 1 (Неисправность датчика наличия продукта 1)
#define MES15 "F_AVAILP2"	// Флаг неисправности датчика наличия продукта 2 (Неисправность датчика наличия продукта 2)
#define MES16 "F_AVAILP3"	// Флаг неисправности датчика наличия продукта 3 (Неисправность датчика наличия продукта 3)
#define MES17 "F_AVAILP4"	// Флаг неисправности датчика наличия продукта 4 (Неисправность датчика наличия продукта 4)

#define MES18 "F_POWER_0"	// Флаг наличия питания (Внешнее электропитание отключено)
#define MES19 "F_SERIAL_0"	// Флаг отсутствия COM порта (COM порт терминала неисправен)
#define MES20 "F_TRAY_0"	// Флаг запрета работы аппарата при несработке датчика в лотке выдачи (Неисправность датчика в лотке выдачи)
#define MES21 "F_SERVER_0"	// Флаг отсутствия связи с сервером (Отсутствие связи с сервером)
#define MES22 "F_TERMIN_0"	// Флаг отсутствия связи с терминалом (Отсутствие связи с терминалом)
#define MES23 "F_ACT1V_0"	// Флаг ДКП 1 актуатора (выдвинут) (ДКП 1 актуатора поз выдвинут)
#define MES24 "F_ACT1Z_0"	// Флаг ДКП 1 актуатора (задвинут) (ДКП 1 актуатора поз задвинут)
#define MES25 "F_ACT2V_0"	// Флаг ДКП 2 актуатора (выдвинут) (ДКП 2 актуатора поз выдвинут)
#define MES26 "F_ACT2Z_0"	// Флаг ДКП 2 актуатора (задвинут) (ДКП 2 актуатора поз задвинут)
#define MES27 "F_ACT3V_0"	// Флаг ДКП 3 актуатора (выдвинут) (ДКП 3 актуатора поз выдвинут)
#define MES28 "F_ACT3Z_0"	// Флаг ДКП 3 актуатора (задвинут) (ДКП 3 актуатора поз задвинут)
#define MES29 "F_ACT4V_0"	// Флаг ДКП 4 актуатора (выдвинут) (ДКП 4 актуатора поз выдвинут)
#define MES30 "F_ACT4Z_0"	// Флаг ДКП 4 актуатора (задвинут) (ДКП 4 актуатора поз задвинут)
#define MES31 "F_AVAILP1_0"	// Флаг неисправности датчика начилия продукта 1
#define MES32 "F_AVAILP2_0"	// Флаг неисправности датчика начилия продукта 2
#define MES33 "F_AVAILP3_0"	// Флаг неисправности датчика начилия продукта 3
#define MES34 "F_AVAILP4_0"	// Флаг неисправности датчика начилия продукта 4
#define MESALL "MESALLERR"	// Сброс ошибок

#define C_TERM 	"C_TERM"
#define C_ADDRESS 	"C_ADDRESS"
#define C_TEL 		"C_TEL"
#define REBOOT 	"REBOOT"

int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen);

void GetSockName(int sockfd, struct sockaddr_in *restrict addr, socklen_t *restrict addrlen);

void Listen(int sockfd, int backlog);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

#endif // SOCKET_FUNCTIONS_H
