#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_INPUT 100
#define DESCRIPTION_SIZE 50
#define USERNAME_SIZE 20
#define ACTIVITY_SIZE 30

#define MAX_USERS 50
#define MAX_TAREFAS 10000
#define MAX_ACTIVITY 10



/* estrutura tarefa */
struct tarefa
{
    int id;
    char description[DESCRIPTION_SIZE];
    char user[USERNAME_SIZE];
    int duration;
    int activity;
    int time;
};

void AddTarefa(struct tarefa tarefas[], int id_tarefa, int time);

int main()
{
    /* inicializacao das variaveis*/
    
    struct tarefa tarefas[MAX_TAREFAS];
    int id_tarefa = 0;  
    int users[MAX_USERS][USERNAME_SIZE];
    int activity[MAX_ACTIVITY][ACTIVITY_SIZE];
    
    int time = 0;
    

    char str[MAX_INPUT];
    char command;
    while((command = getchar()) != 'q'){
        switch(command)
        {
            case('t'):
                id_tarefa++;
                AddTarefa(tarefas, id_tarefa, time);
                break; 
            case('l'):
                scanf("%s", str);
                printf("%s, l", str);
                break; 
            case('n'):
                scanf("%s", str);
                printf("%s, n", str);
                break; 
            case('u'):
                scanf("%s", str);
                printf("%s, u", str);
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
                scanf("%s", str);
                printf("%s, a", str);
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
        printf("too many tasks");
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
            printf("duplicate description");
            return;
        }
    }

    /* Fornecer as restantes caracteristicas da tarefa */
    nova_tarefa.id = id_tarefa;
    nova_tarefa.activity = 0;   /* TO DO = 0 / IN PROGRESS = 1 / DONE = 3 */
    nova_tarefa.time = time;

    /* Inserir a tarefa no vetor */
    tarefas[id_tarefa] = nova_tarefa;
    
    /* saída */
    printf("task %d\n", tarefas[id_tarefa].id);
    return;
}