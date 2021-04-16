/*
 * Ficheiro:  main.c
 * Autor:  Tiago Rodrigues Vieira da Silva
 * Descrição: Projeto 1 de IAED 2020/2021.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/* - Declaração de constantes ----------------------------------------------- */

/* Tamanhos de strings */
#define DESCRIPTION_SIZE 51    /* tamanho descrição */
#define USER_ACTIVITY_SIZE 22  /* tamanho utilizador e atividade */

/* Limites superiores de vetores */
#define MAX_USERS 50          /* máximo de utilizadores */
#define MAX_TAREFAS 10000     /* máximo de tarefas */
#define MAX_ACTIVITY 10       /* máximo de atividades */

/* Limites inferiores de vetores */
#define MIN_TAREFAS 1        /* mínimo de tarefas */
#define MIN_USER_ACTIVITY 0  /* mínimo de utilizadores e atividades */


/* - Declaração da estrutura "tarefa" --------------------------------------- */

struct tarefa
{
    int id;                            /* inteiro de 1 a 10000*/
    char description[DESCRIPTION_SIZE];/* string com maximo de 50 caracteres*/
    char user[USER_ACTIVITY_SIZE];     /* string com maximo de 20 caracteres*/
    int duration;                      /* inteiro positivo*/
    char activity[USER_ACTIVITY_SIZE]; /* string com maximo de 20 caracteres*/
    int time;                          /* inteiro positivo*/
};

/* - Declaração de funções -------------------------------------------------- */

/* Principais */
int AddTarefa(struct tarefa tarefas[], int id_tarefa, 
              char activity[USER_ACTIVITY_SIZE]);
void ListTarefas(struct tarefa tarefas[], int id_tarefa);
int TimeUpdate(int time);
void AddUserActivity(char tab[][USER_ACTIVITY_SIZE], int choice);
void ListActivities(struct tarefa tarefas[], 
                    char activities[][USER_ACTIVITY_SIZE], int id_tarefa);
void MoveTask(struct tarefa tarefas[], char users[][USER_ACTIVITY_SIZE],
              char activities[][USER_ACTIVITY_SIZE], int id_tarefa, int time);

/* Verificadores de argumentos */
int AddTarefa_argcheck(struct tarefa nova_tarefa, struct tarefa tarefas[],
                       int id_tarefa);
int MoveTask_argcheck(char argument[], char tab[][USER_ACTIVITY_SIZE], 
                      int max_size);

/* Auxiliares */
void order_descriptions(struct tarefa temp_tarefas[], int id_tarefa);
void order_time(struct tarefa temp_tarefas[], int counter);
void swaptarefas(struct tarefa temp_tarefas[], int j);


/* - Função Principal --------------------------------------------------------*/
int main()
{
    /* inicializacao das variaveis*/
    
    /* contador */
    int i;
    /*  */
    int id_tarefa = MIN_TAREFAS, time = 0;
    /* vetor de tarefas */
    struct tarefa tarefas[MAX_TAREFAS + 1];  
    /* vetor de utilizadores */
    char users[MAX_USERS][USER_ACTIVITY_SIZE];
    /* vetor de atividades (já inicializado) */
    char activity[MAX_ACTIVITY][USER_ACTIVITY_SIZE] = 
    { "TO DO", "IN PROGRESS", "DONE", "\0", "\0", "\0", "\0", "\0", "\0", "\0"};
    /* char que recebe o comando do input*/
    char command;

    /* inicialização do vetor users */
    for(i = 0; i < MAX_USERS; i++)
        strcpy(users[i], "\0");

    /* analisar comando da linha */
    while((command = getchar()) != 'q'){
        switch(command)
        {
            case('t'):
                id_tarefa = AddTarefa(tarefas, id_tarefa, activity[0]);
                break; 
            case('l'):
                ListTarefas(tarefas, id_tarefa);
                break; 
            case('n'):
                time = TimeUpdate(time);
                break; 
            case('u'):
                AddUserActivity(users, 1);
                break; 
            case('m'):
                MoveTask(tarefas, users, activity, id_tarefa, time);
                break;
            case('d'):
                ListActivities(tarefas, activity, id_tarefa);
                break;   
            case('a'):
                AddUserActivity(activity, 2);
                break;
        }
    }
    return 0;
}

/* - Função que adiciona uma nova tarefa ao sistema (comando t) ------------- */
int AddTarefa(struct tarefa tarefas[], int id_tarefa, 
              char activity[USER_ACTIVITY_SIZE])
{
    /* inicialização das variaveis*/
    /* contador */
    int i;
    /* pointer */
    int p = 0;
    /* tarefa temporaria*/
    struct tarefa nova_tarefa;

    /* entrada */
    scanf("%d", &nova_tarefa.duration);       /* obter a duração do input*/
    scanf("%[^\n]s", nova_tarefa.description);/* obter a descrição do input*/

    /* remover caracteres brancos do início da descrição*/
    for(i = 0; i < USER_ACTIVITY_SIZE; i++)
    {
        if(nova_tarefa.description[i] == ' ' || nova_tarefa.description[i] == '\t')
            p++;
        else
            break;
    }
    memmove(nova_tarefa.description, nova_tarefa.description + p, DESCRIPTION_SIZE);
    
    /* se AddTarefa_argcheck retornar 1 há erros e a função deve parar*/
    if (AddTarefa_argcheck(nova_tarefa, tarefas, id_tarefa) == 1)
        return id_tarefa; /* retornar o id atual */

    /* fornecer as restantes caracteristicas da nova_tarefa */
    nova_tarefa.id = id_tarefa;
    strcpy(nova_tarefa.activity, activity);
    nova_tarefa.time = 0;

    /* inserir a nova_tarefa no vetor tarefas */
    tarefas[id_tarefa] = nova_tarefa;

    /* saída */
    printf("task %d\n", tarefas[id_tarefa].id);
    id_tarefa++;
    return id_tarefa; /* retornar o id da proxima tarefa a ser criada*/
}

/* - Função que avalia os argumentos do input da função "AddTarefa" --------- */
int AddTarefa_argcheck(struct tarefa nova_tarefa, struct tarefa tarefas[],
                       int id_tarefa)
{
    /* inicialização das variaveis*/
    int i;
    
    /* verificar se já se chegou ao maximo de tarefas */
    if (id_tarefa > MAX_TAREFAS)
    {
        printf("too many tasks\n");
        return 1;
    }
    
    /* verificar se já ha uma descricao igual*/
    for(i = MIN_TAREFAS; i < id_tarefa; i++)
    {
        if (strcmp(tarefas[i].description, nova_tarefa.description) == 0)
        {
            printf("duplicate description\n");
            return 1;
        }
    }
    
    /* Verificar se a duracao e positiva */
    if(nova_tarefa.duration <= 0)
    {
        printf("invalid duration\n");
        return 1;
    }
    
    return 0;
}


/* - Função que lista as tarefas (comando l) -------------------------------- */
void ListTarefas(struct tarefa tarefas[], int id_tarefa)
{
    /* inicialização de variaveis*/
    char c;
    /* vetor de ids a apresentar */
    int ids[MAX_TAREFAS];
    /* flags */
    int id_existence = 1;
    /* contadores*/
    int i, k, j;

    /* Verificar se o comando tem argumentos ou não*/
    if((c = getchar()) == ' ')
    {
        /* inserir os argumentos no vetor ids */
        i = MIN_TAREFAS;
        do
        {
        scanf("%d", &ids[i]);
        i++;
        } while (getchar() == ' ');
    }
    else
    {
        /* cópia do array tarefas para o temp_tarefas */
        struct tarefa temp_tarefas[MAX_TAREFAS];
        for (i = MIN_TAREFAS; i < id_tarefa; i++)
            temp_tarefas[i] = tarefas[i];
        
        /* ordenação das descrições por ordem alfabética do temp_tarefas */
        order_descriptions(temp_tarefas, id_tarefa);
        
        /* passar para o array ids os ids das tarefas ja ordenadas */
        for(i = MIN_TAREFAS; i < id_tarefa; i++)
            ids[i] = temp_tarefas[i].id;
    }

    /* apresentar no terminal os ids do vetor id */
    for(k = MIN_TAREFAS; k < i; k++)
    {
        for(j = MIN_TAREFAS; j < id_tarefa; j++)
        {
            if(ids[k] == tarefas[j].id)
            {
                /* saida */
                printf("%d %s #%d %s\n", tarefas[j].id,
                                    tarefas[j].activity,
                                    tarefas[j].duration,
                                    tarefas[j].description);
                id_existence = 0;
            }
        }
        /* se não existir o id a flag "id_existence" estara a 1 */
        if (id_existence == 1)
            printf("%d: no such task\n", ids[k]);
        id_existence = 1;
    }
}


/* - Função que atualiza a variável time (comando n) ------------------------ */
int TimeUpdate(int time)
{
    /* inicialização variaveis */
    int update;
    
    /* input do terminal*/
    scanf(" %d", &update);

    /* Verificar se o input e inteiro positivo */   
    if(update < 0)
        {
            printf("invalid time\n");
            return time;
        }
    
    time += update;
    
    /* saida */
    printf("%d\n", time);
    
    return time;
}

/* - Função que adiciona um user ou activity (comando u ou a) --------------- */
void AddUserActivity(char tab[][USER_ACTIVITY_SIZE], int choice)
{
    /* inicialização das variáveis */
    char c;
    /* string do input*/
    char temp_str[USER_ACTIVITY_SIZE];
    /* flags */
    int max_check = 0;
    /* contadores */
    int i; 
    /* pointers */
    int p = 0;
    /* valore maximo do vetor*/
    int max;
    
    /* choice == 1: adicionar user */
    /* choice == 2: adicionar activity */
    if(choice == 1)
        max = MAX_USERS;
    else if (choice == 2)
        max = MAX_ACTIVITY;

    /* verificar se o comando tem argumentos */
    if((c = getchar()) == ' ')
    {   
        /* argumentos do input*/
        fgets(temp_str, USER_ACTIVITY_SIZE, stdin);
        
        /* remover caracteres brancos do inicio da string se for para adicionar
           um user */
        if (choice == 1)
        {
            for(i = 0; i < USER_ACTIVITY_SIZE; i++)
            {
                if(temp_str[i] == ' ' || temp_str[i] == '\t')
                    p++;
                else
                    break;
            }
        memmove(temp_str, temp_str + p, USER_ACTIVITY_SIZE);
        }

        /* remover o /n da string e verificar que não há minúsculas*/
        for (i = 0; i < USER_ACTIVITY_SIZE; i++)
        {
            if(temp_str[i] == '\n')
            {
                temp_str[i] = '\0';
                break;
            }
            /* só verifica as minusculas se for uma activity*/
            else if('a' <= temp_str[i] && temp_str[i] <= 'z' && choice == 2)
            {
                printf("invalid description\n");
                return;
            }
        }
        
        /* verificar se os argumentos já existem no array */
        for(i = MIN_USER_ACTIVITY; i < max; i++)
        {
            if(strcmp(temp_str, tab[i]) == 0)
            {
                if(choice == 1)
                    printf("user already exists\n");
                else if (choice == 2)
                    printf("duplicate activity\n");
                return;
            }
            /* inserir a string na posição vazia do tab e ativar a flag 
                max_check*/
            else if(strcmp(tab[i], "\0") == 0)
            {
                strcpy(tab[i], temp_str);
                max_check = 1;
                break;
            }
        }
        /* exibir mensagem de erro se a flag max_check nao foi ativada*/
        if(max_check == 0)
        {
            if(choice == 1)
            {
                printf("too many users\n");
                return;
            }
            else if (choice == 2)
            {
                printf("too many activities\n");
                return;
            }
        }
    }
    else
    {
       /* listar users/atividades */
       i = MIN_USER_ACTIVITY;
       while(strcmp(tab[i], "\0") != 0 && i < max)
       {
           printf("%s\n", tab[i]);
           i++;
       }
    }
}

/* - Função que adiciona move uma tarefa entre activities (comando m) ------- */
void MoveTask(struct tarefa tarefas[], char users[][USER_ACTIVITY_SIZE],
                char activities[][USER_ACTIVITY_SIZE], int id_tarefa, int time)
{
    /* inicialização de variavéis*/
    /* inputs*/
    int id;
    char user[USER_ACTIVITY_SIZE], activity[USER_ACTIVITY_SIZE];

    /* guardar o input */
    scanf("%d %s %[^\n]s", &id, user, activity);
    
    /* verificar se o id da tarefa e valido */
    if(id_tarefa <= id || id <= 0)
    {
        printf("no such task\n");
        return;
    }
    
    /* verificar se a atividade da tarefa é a TO DO */
    if (strcmp(tarefas[id].activity, activity) == 0)
    {
        return;
    }

    /* verificar se a atividade nao e a "TO DO" */
    if (strcmp(activities[0], activity) == 0)
    {
        printf("task already started\n");
        return;
    }

    /* verificar se o utilizador introduzido existe */
    if(MoveTask_argcheck(user, users, MAX_USERS) != 0)
    {
        printf("no such user\n");
        return;
    }

    /* verificar se a atividade introduzida existe */
    if(MoveTask_argcheck(activity, activities, MAX_ACTIVITY) != 0)
    {
        printf("no such activity\n");
        return;
    }

    /* se a tarefa estiver inicialmente em "TO DO" marcar o tempo */
    if(strcmp(tarefas[id].activity, activities[0]) == 0)
        tarefas[id].time = time;
    
    /* se a tarefa for para "DONE" output da duração e slack */
    if(strcmp(activity, activities[2]) == 0)
    {
        int slack, tarefa_duration;
        tarefa_duration = time - tarefas[id].time;
        slack = tarefa_duration - tarefas[id].duration;
        printf("duration=%d slack=%d\n", tarefa_duration, slack);
    }

    /* guardar a activity e o user na tarefa */
    strcpy(tarefas[id].activity, activity);
    strcpy(tarefas[id].user, user);
}


/* - Função que verifica a existencia dos inputs do "MoveTask" nos vetores -- */
int MoveTask_argcheck(char argument[], char tab[][USER_ACTIVITY_SIZE], int max_size)
{
    /* inicialização de variáveis*/
    int i = 0;

    /* verificar a existencia do argumento no vetor*/
    for(i = 0; i < max_size; i++)
    {
        if(strcmp(tab[i], argument) == 0)
            return 0;
    }
    return 1;
}

/* - Função que lista as tarefas numa dada activity (comando d) ------------- */
void ListActivities(struct tarefa tarefas[], char activities[][USER_ACTIVITY_SIZE], int id_tarefa)
{
    /* inicializacao de variaveis */
    /* contadores */
    int i, temp_counter = MIN_TAREFAS;
    /* activity de input */
    char activity[USER_ACTIVITY_SIZE];
    /* tarefa temporaria*/
    struct tarefa temp_tarefas[MAX_TAREFAS];
    
    /* guardar input */
    getchar();
    scanf("%[^\n]s", activity);
    
    /* verificar argumento */
    if(MoveTask_argcheck(activity, activities, MAX_ACTIVITY) != 0)
    {
        printf("no such activity\n");
        return;
    }

    /* mudar para um array temporario as tarefas em certa atividade */
    for(i = MIN_TAREFAS; i < id_tarefa; i++)
    {
        if(strcmp(tarefas[i].activity, activity) == 0)
        {
            temp_tarefas[temp_counter] = tarefas[i];
            temp_counter++;
        }
    }

    /* organizar o array */
    order_descriptions(temp_tarefas, temp_counter);
    order_time(temp_tarefas, temp_counter);
    /* saida */
    for(i = MIN_TAREFAS; i < temp_counter; i++)
        printf("%d %d %s\n", temp_tarefas[i].id, temp_tarefas[i].time, 
                             temp_tarefas[i].description);
}


/* - Funcao auxiliar para ordenar as tarefas por ordem alfabetica ----------- */
void order_descriptions(struct tarefa temp_tarefas[], int id_tarefa)
{
    /* inicializacao de variaveis */
    int i, j, end;
    for (i = MIN_TAREFAS; i < id_tarefa; i++)
    {
        end = 1;
        for(j = MIN_TAREFAS; j < id_tarefa - i; j++)
        {
            if (strcmp(temp_tarefas[j].description ,temp_tarefas[j + 1].description) > 0)
            {
                /* so precisamos de ordenar as descricoes e os ids */
                swaptarefas(temp_tarefas, j);
                end = 0;
            }
        }
        if (end == 1)
            break;
    }
}

/* - Funcao auxiliar para ordenar as tarefas por tempo de inicio ------------ */
void order_time(struct tarefa temp_tarefas[], int counter)
{
    /* inicializacao de variaveis */
    int i, j, end;
    for (i = MIN_TAREFAS; i < counter; i++)
    {
        end = 1;
        for(j = MIN_TAREFAS; j < counter - i; j++)
        {
            if (temp_tarefas[j].time > temp_tarefas[j + 1].time)
            {
                swaptarefas(temp_tarefas, j);
                end = 0;
            }
        }
        if (end == 1)
            break;
    }
}

/* - Função auxiliar que troca um elemento do array com o seguinte ---------- */
void swaptarefas(struct tarefa temp_tarefas[], int j)
{
    struct tarefa temp_tarefa; 

    temp_tarefa = temp_tarefas[j];
    temp_tarefas[j] = temp_tarefas[j + 1];
    temp_tarefas[j + 1] = temp_tarefa; 
}