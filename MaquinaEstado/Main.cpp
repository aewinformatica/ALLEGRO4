#include "Global.h" 

int main()
{
  // Cria a máquina
  Maquina* maquina = Maquina::CriaInstancia();

  // Cria os estados
  EstadoApresentacao* estadoApresentacao = EstadoApresentacao::CriaInstancia();
  EstadoMenu* estadoMenu = EstadoMenu::CriaInstancia();
  EstadoJogo* estadoJogo = EstadoJogo::CriaInstancia();
  
  // Seta estado inicial
  maquina->ExecutaTransicao(estadoApresentacao);

  // Tempo
  time_t timer_start;
  time_t timer_current;
  
  timer_start = time(NULL);
  
  double diff = 0;
  
  // Loop principal do jogo
  while(1)
  {
    timer_current = time(NULL);

    diff = difftime(timer_current, timer_start);
    
    // Implementação dos eventos: teclas ESC, j e q, e evento de tempo terminado
    if(kbhit())
    {
      int tecla = getch();

      if(tecla == 27) // tecla ESC
      {
        maquina->AoPressionarESC();
      }
      if(tecla == 106) // tecla j
      {
        maquina->AoPressionarJogar();
      }
      if(tecla == 113) // tecla q
      {
        maquina->AoPressionarSair();
      }
    }

    if(diff >= 5)
    {
      maquina->AoTerminarTempo();
    }

    // Finaliza a aplicação caso tenham apertado 'q' no menu
    if(maquina->PodeFinalizar())
    {
      break;
    }

    // Limpa a tela
    system("cls");
    
    // Desenha na tela
    printf("Loop: %4.2f\n", diff);

    // Frame
    maquina->ExecutaFrame();
  }

  // Libera a memória dos objetos
  delete estadoJogo;
  delete estadoMenu;
  delete estadoApresentacao;
  delete maquina;

  return 0;
} 
