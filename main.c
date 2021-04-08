#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_INPUT 100
#define DESCRIPTION_SIZE 51
#define USER_ACTIVITY_SIZE 21

#define MAX_USERS 50
#define MAX_TAREFAS 10000
#define MAX_ACTIVITY 10

#define MIN_TAREFAS 1

#define MIN_USER_ACTIVITY 0



/* estrutura tarefa */
struct tarefa
{
    int id;
    char description[DESCRIPTION_SIZE];
    char user[USER_ACTIVITY_SIZE];
    int duration;
    char activity[USER_ACTIVITY_SIZE];
    int time;
};

int AddTarefa(struct tarefa tarefas[], int id_tarefa, char activity[USER_ACTIVITY_SIZE]);
void ListTarefas(struct tarefa tarefas[], int id_tarefa);
void order_descriptions(struct tarefa temp_tarefas[], int id_tarefa);
int TimeUpdate(int time);
void AddUserActivity(char tab[][USER_ACTIVITY_SIZE], int choice);
void MoveTask(struct tarefa tarefas[], char users[][USER_ACTIVITY_SIZE],
                char activities[][USER_ACTIVITY_SIZE], int id_tarefa, int time);
int MoveTask_argcheck(char argument[], char tab[][USER_ACTIVITY_SIZE], int max_size);
void ListActivities(struct tarefa tarefas[], char activities[][USER_ACTIVITY_SIZE], int id_tarefa);



int main()
{
    /* inicializacao das variaveis*/
    
    struct tarefa tarefas[MAX_TAREFAS + 1];
    int i, id_tarefa = MIN_TAREFAS;  
    
    char users[MAX_USERS][USER_ACTIVITY_SIZE];
    char activity[MAX_ACTIVITY][USER_ACTIVITY_SIZE] = 
    { "TO DO", "IN PROGRESS", "DONE", "\0", "\0", "\0", "\0", "\0", "\0", "\0"};
    int time = 0;
    char command;

    for(i = 0; i < MAX_USERS; i++)
        strcpy(users[i], "\0");

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


int AddTarefa(struct tarefa tarefas[], int id_tarefa, char activity[USER_ACTIVITY_SIZE])
{
    int i;
    /* Criar nova tarefa para depois ser introduzida no vector das tarefas */
    struct tarefa nova_tarefa;

    /* entrada */
    scanf("%d", &nova_tarefa.duration);
    getchar();
    scanf("%[^\n]s", nova_tarefa.description);


    /* Verificar se já se chegou ao maximo de tarefas */
    if (id_tarefa > MAX_TAREFAS)
    {
        printf("too many tasks\n");
        return id_tarefa;
    }
    
    /* Verificar se já ha uma descricao igual*/
    for(i = MIN_TAREFAS; i < id_tarefa; i++)
    {
        if (strcmp(tarefas[i].description, nova_tarefa.description) == 0)
        {
            printf("duplicate description\n");
            return id_tarefa;
        }
    }
    
    /* Verificar se a duracao e positiva */
    if(nova_tarefa.duration <= 0)
        {
        printf("invalid duration\n");
        return id_tarefa;
        }

    /* Fornecer as restantes caracteristicas da tarefa */
    nova_tarefa.id = id_tarefa;
    strcpy(nova_tarefa.activity, activity);
    nova_tarefa.time = 0;

    /* Inserir a tarefa no vetor */
    tarefas[id_tarefa] = nova_tarefa;

    /* saída */
    printf("task %d\n", tarefas[id_tarefa].id);
    id_tarefa++;
    return id_tarefa;
}


void ListTarefas(struct tarefa tarefas[], int id_tarefa)
{
    /* Inicializacao de variaveis*/
    char c;
    int ids[MAX_TAREFAS];
    int i, k, j;
        /* Variavel para verificar se o ID existe */
    int id_existence = 1;

    /* Verificar se o comando tem argumentos*/
    if((c = getchar()) == ' ')
    {
        i = MIN_TAREFAS;
        do
        {
        scanf("%d", &ids[i]);
        i++;
        } while (getchar() == ' ');
    }
    else
    {
        /* Inicializacao da copia do array de tarefas */
        struct tarefa temp_tarefas[MAX_TAREFAS];
        for (i = MIN_TAREFAS; i < id_tarefa; i++)
            temp_tarefas[i] = tarefas[i];
        
        /* Ordenacao das descricoes por ordem alfabetica */
        order_descriptions(temp_tarefas, id_tarefa);
        
        /* passar para o array os ids das tarefas ja ordenadas */
        for(i = MIN_TAREFAS; i < id_tarefa; i++)
            ids[i] = temp_tarefas[i].id;
    }

    /* Apresentar no terminal os ids que interessam */
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
        /* Se nao existir o ID variavel "id_existence" estara a 1 */
        if (id_existence == 1)
            printf("%d: no such task\n", ids[k]);
        id_existence = 1;
    }
}


/* Funcao auxiliar para ordenar as tarefas por ordem alfabetica */
void order_descriptions(struct tarefa temp_tarefas[], int id_tarefa)
{
    char temp_str[DESCRIPTION_SIZE];
    int i, j, temp_id;
    for(i = MIN_TAREFAS; i < id_tarefa; i++)
    {
        for(j = i + 1; j < id_tarefa; j++)
        {
            if(strcmp(temp_tarefas[i].description ,temp_tarefas[j].description)>0)
            {
                /* so precisamos de ordenar as descricoes e os ids */
                strcpy(temp_str, temp_tarefas[i].description);
                temp_id = temp_tarefas[i].id;
                strcpy(temp_tarefas[i].description, temp_tarefas[j].description);
                temp_tarefas[i].id = temp_tarefas[j].id;
                strcpy(temp_tarefas[j].description, temp_str);
                temp_tarefas[j].id = temp_id;
            }
        }
    }
}


int TimeUpdate(int time)
{
    /* Inicializar variaveis */
    int update;
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


void AddUserActivity(char tab[][USER_ACTIVITY_SIZE], int choice)
{
    /* Inicializar variaveis */
    char c;
    char temp_str[USER_ACTIVITY_SIZE];
    int max_check = 0;
    int max, i;
    
    /* choice == 1: editar users
       choice == 2: editar activities */
    if(choice == 1)
        max = MAX_USERS;
    else if (choice == 2)
        max = MAX_ACTIVITY;

    /* Verificar se o comando tem argumentos */
    if((c = getchar()) == ' ')
    {
        fgets(temp_str, USER_ACTIVITY_SIZE, stdin);

        for (i = 0; i < USER_ACTIVITY_SIZE; i++)
        {
            /* analisar os argumentos do terminal */
            if(temp_str[i] == '\n')
            {
                temp_str[i] = '\0';
                break;
            }
            else
            {
                if('a' <= temp_str[i] && temp_str[i] <= 'z' && choice == 2)
                {
                    printf("invalid description\n");
                    return;
                }
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
            else if(strcmp(tab[i], "\0") == 0)
            {
                strcpy(tab[i], temp_str);
                max_check = 1;
                break;
            }
        }
        /* exibir mensagem de erro se o array esta cheio */
        if(max_check == 0)
        {
            if(choice == 1)
            {
                printf("too many users\n");
                getchar();
            }
            else if (choice == 2)
                printf("too many activities\n");
        }
    }
    else    /* listar users/atividades */
    {
       i = MIN_USER_ACTIVITY;
       while(strcmp(tab[i], "\0") != 0 && i < max)
       {
           printf("%s\n", tab[i]);
           i++;
       }
    }
}

void MoveTask(struct tarefa tarefas[], char users[][USER_ACTIVITY_SIZE],
                char activities[][USER_ACTIVITY_SIZE], int id_tarefa, int time)
{
    int id;
    char user[USER_ACTIVITY_SIZE], activity[USER_ACTIVITY_SIZE];

    scanf("%d %s %[^\n]s", &id, user, activity);

    /* verificar se o id da tarefa e valido */
    if(id_tarefa <= id)
    {
        printf("no such task\n");
        return;
    }

    /* verificar se a atividade nao e a "TO DO" */
    if (strcmp(activities[0], activity) == 0)
    {
        printf("task already started\n");
        return;
    }

    /* verificar se o utilizador introduzidO existe */
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
    
    /* se a tarefa for para "DONE" */
    if(strcmp(activity, activities[2]) == 0)
    {
        int slack, tarefa_duration;
        tarefa_duration = time - tarefas[id].time;
        slack = tarefa_duration - tarefas[id].duration;
        printf("duration=%d slack=%d\n", tarefa_duration, slack);
    }

    strcpy(tarefas[id].activity, activity);
    strcpy(tarefas[id].user, user);
}

/* funcao que verifica se o user e a activity existem */
int MoveTask_argcheck(char argument[], char tab[][USER_ACTIVITY_SIZE], int max_size)
{
    int i = 0;

    for(i = 0; i < max_size; i++)
    {
        if(strcmp(tab[i], argument) == 0)
            return 0;
    }
    return 1;
}


void ListActivities(struct tarefa tarefas[], char activities[][USER_ACTIVITY_SIZE], int id_tarefa)
{
    /* Inicializacao de variaveis*/
    char activity[USER_ACTIVITY_SIZE];
    int i, j, temp_counter = MIN_TAREFAS, temp_id, temp_time;
    struct tarefa temp_tarefas[MAX_TAREFAS];
    char temp_str[DESCRIPTION_SIZE];
    
    /* buscar argumento */
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
            temp_tarefas[temp_counter].id = tarefas[i].id;
            temp_tarefas[temp_counter].time = tarefas[i].time;
            strcpy(temp_tarefas[temp_counter].description, tarefas[i].description);
            temp_counter++;
        }
    }

    /* organizar o array */
    for(i = MIN_TAREFAS; i < temp_counter; i++)
    {
        for(j = i + 1; j < temp_counter; j++)
        {
            if( temp_tarefas[i].time > temp_tarefas[j].time || 
                (temp_tarefas[i].time == temp_tarefas[j].time 
                &&
                (strcmp(temp_tarefas[i].description ,temp_tarefas[j].description)) > 0))
            {
                /* so precisamos de ordenar as descricoes, o tempo e os ids */
                strcpy(temp_str, temp_tarefas[i].description);
                temp_id = temp_tarefas[i].id;
                temp_time = temp_tarefas[i].time;
                strcpy(temp_tarefas[i].description, temp_tarefas[j].description);
                temp_tarefas[i].id = temp_tarefas[j].id;
                temp_tarefas[i].time = temp_tarefas[j].time;
                strcpy(temp_tarefas[j].description, temp_str);
                temp_tarefas[j].id = temp_id;
                temp_tarefas[j].time = temp_time;
            }
        }
    }

    /* saida */
    for(i = MIN_TAREFAS; i < temp_counter; i++)
        printf("%d %d %s\n", temp_tarefas[i].id, temp_tarefas[i].time, temp_tarefas[i].description);
}