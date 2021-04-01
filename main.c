#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_INPUT 100
#define DESCRIPTION_SIZE 50
#define USER_ACTIVITY_SIZE 20

#define MAX_USERS 50
#define MAX_TAREFAS 10000
#define MAX_ACTIVITY 10



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

void AddTarefa(struct tarefa tarefas[], int id_tarefa, int time);
void ListTarefas(struct tarefa tarefas[], int id_tarefa, char activity[][USER_ACTIVITY_SIZE]);
void order_descriptions(struct tarefa temp_tarefas[], int id_tarefa);
int TimeUpdate(int time);
void AddUserActivity(char tab[][USER_ACTIVITY_SIZE], int choice);

int main()
{
    /* inicializacao das variaveis*/
    
    struct tarefa tarefas[MAX_TAREFAS];
    int id_tarefa = 0;  
    
    char users[MAX_USERS][USER_ACTIVITY_SIZE]; 
    for(int i = 0; i < MAX_USERS; i++)
        strcpy(users[i], "\0");

    char activity[MAX_ACTIVITY][USER_ACTIVITY_SIZE] = 
    { "TO DO", "IN PROGRESS", "DONE", "\0", "\0", "\0", "\0", "\0", "\0", "\0"};
    int time = 0;
    
    char str[MAX_INPUT];
    char command;
    while((command = getchar()) != 'q'){
        switch(command)
        {
            case('t'):
                AddTarefa(tarefas, id_tarefa, time);
                id_tarefa++;
                break; 
            case('l'):
                ListTarefas(tarefas, id_tarefa, activity);
                break; 
            case('n'):
                time = TimeUpdate(time);
                break; 
            case('u'):
                AddUserActivity(users, 1);
                break; 
            case('m'):
                scanf("%s", str);
                printf("%s, m", str);
                break;; 
            case('d'):
                scanf("%s", str);
                printf("%s, d", str);
                break;   
            case('a'):
                AddUserActivity(activity, 2);
                break;  
        }
    }
    return 0;
}


void AddTarefa(struct tarefa tarefas[], int id_tarefa, int time)
{
    /* Verificar se já se chegou ao maximo de tarefas */
    if (id_tarefa > MAX_TAREFAS)
    {
        printf("too many tasks\n");
        return;
    }
    
    /* Criar nova tarefa para depois ser introduzida no vector das tarefas */
    struct tarefa nova_tarefa;

    /* entrada */
    scanf(" %d %[^\n]s", &nova_tarefa.duration, nova_tarefa.description);
    
    /* Verificar se já ha uma descricao igual*/
    for(int i; i < id_tarefa; i++)
    {
        if (strcmp(tarefas[i].description, nova_tarefa.description))
        {
            printf("duplicate description\n");
            return;
        }
    }
    
    /* Verificar se a duracao e positiva */
    if(nova_tarefa.duration <= 0)
        {
        printf("invalid duration\n");
        return;
        }

    /* Fornecer as restantes caracteristicas da tarefa */
    nova_tarefa.id = id_tarefa;
    strcpy(nova_tarefa.activity, "TO DO");
    nova_tarefa.time = time;

    /* Inserir a tarefa no vetor */
    tarefas[id_tarefa] = nova_tarefa;
    
    /* saída */
    printf("task %d\n", tarefas[id_tarefa].id);
    return;
}


void ListTarefas(struct tarefa tarefas[], int id_tarefa, char activity[][USER_ACTIVITY_SIZE])
{
    /* Inicializacao de variaveis*/
    int ids[MAX_TAREFAS];
    char c;
    int i = 0;

    /* Verificar se o comando tem argumentos*/
    if((c = getchar()) == ' ' && (c = getchar()) == '[')
    {
        do
        {
        scanf("%d", &ids[i]);
        i++;
        } while (getchar() != ']');
    }
    else
    {
        /* Inicializacao da copia do array de tarefas */
        struct tarefa temp_tarefas[MAX_TAREFAS];
        for (i = 0; i < id_tarefa; i++)
            temp_tarefas[i] = tarefas[i];
        
        /* Ordenacao das descricoes por ordem alfabetica */
        order_descriptions(temp_tarefas, id_tarefa);
        
        /* passar para o array os ids das tarefas ja ordenadas */
        for(i = 0; i < id_tarefa; i++)
            ids[i] = temp_tarefas[i].id;
    }

    /* Apresentar no terminal os ids que interessam */
    
    /* Variavel para verificar se o ID existe */
    int id_existence = 1;
    for(int k = 0; k < i; k++)
    {
        for(int j = 0; j < id_tarefa; j++)
        {
            if(ids[k] == tarefas[j].id)
            {
                /* saida */
                printf("%d %s %d %s\n", tarefas[j].id,
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
    int temp_id;
    for(int i=0; i < id_tarefa; i++)
    {
        for(int j = i+1; j < id_tarefa; j++)
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
    char temp_tab[USER_ACTIVITY_SIZE];
    int max_check = 0;
    int max;
    
    /* choice == 1: editar users
       choice == 2: editar activities */
    if(choice == 1)
        max = MAX_USERS;
    else if (choice == 2)
        max = MAX_ACTIVITY;

    /* Verificar se o comando tem argumentos */
    if((c = getchar()) == ' ' && (c = getchar()) == '[')
    {
        for(int i = 0; i < USER_ACTIVITY_SIZE; i++)
        {
            /* analisar os argumentos do terminal */
            c = getchar();
            if(c == ']')
                break;
            else
            {
                if('a' < c && c < 'z' && choice == 2)
                {
                    printf("invalid description\n");
                    return;
                }
                temp_tab[i] = c;
            }
        }
        /* verificar se os argumentos já existem no array */
        for(int i = 0; i < max; i++)
        {
            if(strcmp(temp_tab, tab[i]) == 0)
            {
                if(choice == 1)
                    printf("user already exists\n");
                else if (choice == 2)
                    printf("duplicate activity\n");
                return;
            }
            else if(strcmp(tab[i], "\0") == 0)
            {
                strcpy(tab[i], temp_tab);
                max_check = 1;
                break;
            }
        }
        /* exibir mensagem de erro se o array esta cheio */
        if(max_check == 0)
        {
            if(choice == 1)
                printf("too many users\n");
            else if (choice == 2)
                printf("too many activities\n");
        }
    }
    else    /* listar users/atividades */
    {
       int i = 0;
       while(strcmp(tab[i], "\0") != 0 && i < max)
       {
           printf("%s\n", tab[i]);
           i++;
       }
    }
}

