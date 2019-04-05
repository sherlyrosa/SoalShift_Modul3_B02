#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

char petname[77];
int hunger_stat = 200;
//sekaligus nilai maks, berkurang 5 tiap 10detik, jika 0 = gameover
//pemain bisa memberi makan
int food_strg = 0;
//makanan dibeli di market
int hygiene_stat = 100; //berkurang 10 tiap 30detik
//bertambah 30 jika dimandikan, cooldown 20detik
int bath_time = 0;
int health_stat = 300; //regenerasi 5pt tiap 10detik, ketika standby
int health_enemy = 100; //nyawa musuh
int pet_stat = 0; //0 = standby, 1 = battle, 2 = shop
int NPC_health = 100; //serangan 20, serangan bergantian.

char getch();

void *Hunger(void *argv);
void *Hygiene(void *argv);
void *Bathing(void *argv);
void *Regenerating(void *argv);
void *menu0(void *argv);
void *menu1(void *argv);

int main(int argc, char const *argv[]) {
  printf("Kasih Nama: ");
  scanf("%s", petname);

  pthread_t tid1, tid2, tid3, tid4, tid5, tid6;

  pthread_create(&tid1, NULL, Hunger, NULL);
  pthread_create(&tid2, NULL, Hygiene, NULL);
  pthread_create(&tid3, NULL, Bathing, NULL);
  pthread_create(&tid4, NULL, Regenerating, NULL);
  pthread_create(&tid5, NULL, menu0, NULL);
  pthread_create(&tid6, NULL, menu1, NULL);

  while (1) {
    if(pet_stat == 0){
      printf("Standby Mode\n");
      printf("Health\t\t: %d\n", health_stat);
      printf("Hunger\t\t: %d\n", hunger_stat);
      printf("Hygiene\t\t: %d\n", hygiene_stat);
      printf("Food left\t: %d\n", food_strg);
      if(bath_time == 0)
        printf("Bath is ready\n");
      else
        printf("Bath will be ready in %ds\n", bath_time);
      printf("Choices\n");
      printf("1. Eat\n");
      printf("2. Bath\n");
      printf("3. Battle\n");
      printf("4. Shop\n");
      printf("5. Exit\n");
    }else if (pet_stat == 1){
      printf("Battle Mode\n");
      printf("Monster's Health: %d\n", health_stat);
      printf("Enemy's Health\t: %d\n", health_enemy);
      printf("Choices\n");
      printf("1. Attack\n");
      printf("2. Run\n");
    }else if(pet_stat == 2){
      printf("Shop Mode\n");
      printf("Shop food stock\t: \n");
      printf("Your food stock\t: \n");
      printf("Choices\n");
      printf("1. Buy\n");
      printf("2. Back\n");
    }
    sleep(1);
    system("clear");
  }

  pthread_join(tid1, NULL);
//  pthread_join(tid2, NULL);
//  pthread_join(tid3, NULL);

  return 0;
}

void *Hunger(void *argv){
  while (1) {
    if(hunger_stat > 200) hunger_stat = 200;

    sleep(10);
    hunger_stat-=5;
    printf("hunger_stat: %d\n", hunger_stat);
    if(hunger_stat <= 0){
      printf("Lapar - Game Over\n");
      exit(0);
    }
  }
}

void *Hygiene(void *argv){
  while (1) {
    sleep(10);
    hygiene_stat-=10;

    if(hygiene_stat <= 0){
      printf("Kotor - Game Over\n");
      exit(0);
    }
  }
}

void *Bathing(void *argv){
  while (1) {
    if (bath_time != 0) {
      hygiene_stat+=30;
      while(bath_time != 0) {
        sleep(1);
        bath_time-=1;
      }
    }
  }
}

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

void *menu0(void *argv) {
  char in;
  while (1) {
    in = getch();
    if(pet_stat == 0){
      if(in == '1'){
        //Mode makan
        printf("1\n");
      }else if(in == '2'){
        //Mode mandi
        printf("mandi dulu\n");
        if(bath_time == 0) bath_time = 20;
      }else if(in == '3'){
        //Mode tarung
        printf("3\n");
        pet_stat = 1;
      }else if(in == '4'){
        //Mode pasar
        printf("4\n");
      }else if(in == '5'){
        //keluar game
        printf("exit . . .\n");
        exit(0);
      }
    }
  }
}

void *menu1(void *argv){
  //menu tarung
  char in;
  while (1) {
    in = getch();
    if(pet_stat == 1){
      if(in == '1'){
        printf("%s is attacking\n", petname);
        sleep(1);
        health_enemy -= 20;
        printf("Enemy is attacking\n");
        sleep(1);
        health_stat -= 20;
      }else if(in == '2'){
        //Mode kabur
        printf("%s is running\n", petname);
        health_enemy = 100;
        pet_stat = 0;
      }
    }
  }
}

void *Regenerating(void *argv) {
  while (1) {
    if(pet_stat == 0 && health_stat < 30){
      health_stat+=5;
      sleep(10);
    }
  }
}
