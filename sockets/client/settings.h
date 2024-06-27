#ifndef SETTING_H
#define SETTING_H

#define ENABEL_PRICE_NAME_ITEM // закоментируй это если хочешь указать только цену на товар

#ifdef ENABEL_PRICE_NAME_ITEM

#define PRICE1 300 // в младших денежных единицах
#define NAME1 "КОФЕ"
#define IDITEM1 0

#define PRICE2 300 // в младших денежных единицах
#define NAME2 "ШАРФ"
#define IDITEM2 1

#define PRICE3 300 // в младших денежных единицах
#define NAME3 "ПАЛЬТО"
#define IDITEM3 2

#define PRICE4 300 // в младших денежных единицах
#define NAME4 "КУРТКА"
#define IDITEM4 3

#else

#define PRICE1 500
#define PRICE2 500
#define PRICE3 500
#define PRICE4 500

#endif

#define VOLUME_VALUE               1  // от 0 до 30, инчае дефолт макс
#define REPEAT_WARE_ABHOLEN        5 // количество повторений
#define BILDDAUER_LEER_VOLL        10000 // пустой или полный время изображение на экранне in ms

//#define DROP_ITMS                  10000 //продолжительность ожидания падения товара in ms

#endif //setting






//******************************************************************************
/*void dispaly_row(MYSQL *ptr)
{
    char str[1024];
    unsigned int field_count = 0;
    while(field_count<mysql_field_count(ptr))
    {
        snprintf(str,1024,"%s",sqlrow[field_count]);
        printf("%s %d \n",str,field_count);
        field_count++;
    }
    printf("\n");
}  */

//*****************************************************************************
// конвертирование строки запроса для записи чеков в БД
//*****************************************************************************
/*char *CreateMesCheck(char *bankcheck,int NumOfCheck)
{

  char  s0[1024]="INSERT INTO CHECK_(CH1,CH2,CH3,CH4,CH_ERR,DTIME) VALUES('', '', '', '', '', NOW())";
  char  s1[]="', '', '', '', '', NOW())";
  char  s2[]="', '', '', '', NOW())";
  char  s3[]="', '', '', NOW())";
  char  s4[]="', '', NOW())";
  char  s5[]="', NOW())";

  memset (ArrForCopy,0, 1024);
  strcpy (ArrForCopy, s0);
  switch(NumOfCheck)
  {

    case 1: {memmove (&ArrForCopy[57], &bankcheck[0], strlen(bankcheck));
             memmove (&ArrForCopy[57+strlen(bankcheck)], &s1[0], strlen(s1)); break;}
    case 2: {memmove (&ArrForCopy[61], &bankcheck[0], strlen(bankcheck));
             memmove (&ArrForCopy[61+strlen(bankcheck)], &s2[0], strlen(s2)); break;}
    case 3: {memmove (&ArrForCopy[65], &bankcheck[0], strlen(bankcheck));
             memmove (&ArrForCopy[65+strlen(bankcheck)], &s3[0], strlen(s3)); break;}
    case 4: {memmove (&ArrForCopy[69], &bankcheck[0], strlen(bankcheck));
             memmove (&ArrForCopy[69+strlen(bankcheck)], &s4[0], strlen(s4)); break;}
    case 5: {memmove (&ArrForCopy[73], &bankcheck[0], strlen(bankcheck));
             memmove (&ArrForCopy[73+strlen(bankcheck)], &s5[0], strlen(s5)); break;}
  }
  return ArrForCopy;
}  */

//******************************************************************************
//конвертирование строки запроса в БД
//******************************************************************************
/*void ConvertMessage(char *mes,int flag_mes)
{
  int i;

  if (flag_mes==1)
  {
    for (i=0;i<17;i++)
    {
      if (ArrOfErr[i])   {mes[190+2*i]='1';} else mes[190+2*i]='0';
    }
  }


  if (flag_mes==2)
  {
    for (i=0;i<4;i++ )
    {
     if (!AvailProd[i+1])  {mes[73+4*i]='0';mes[73+4*i+1]='0';mes[73+4*i+2]='0';}
     else
       {
         mes[73+4*i]=  ((char)(AvailProd[1+i]%1000/100)+48);
         mes[73+4*i+1]=((char)(AvailProd[1+i]%100/10)+48);
         mes[73+4*i+2]=((char)(AvailProd[1+i]%10)+48);
       }
    }
  }
}

// Последние записи в таблице можно вывести следующим образом при наличии первичного ключа:
// SELECT * FROM table_name ORDER BY id DESC LIMIT 100;
// C помощью LIMIT указываем необходимое нам количество последних записей (в примере выше - 100).

/*
  Делаем полную выборку из таблицы your_table с помощью оператора SELECT,
  используем сортирову в обратном порядке по столбцу id (ORDER BY id DESC)
  и оставляем одну запись (LIMIT 1), понятно что она и будет последней добавленной записью.
  SELECT * FROM your_table ORDER BY id DESC LIMIT 1;
  Получение последнего добавленного id в таблице mysql - способ 2.
  Используем для получения последнего id вставленной строки, функцию LAST_INSERT_ID().
  SELECT * FROM your_table WHERE id=LAST_INSERT_ID();
*/

 