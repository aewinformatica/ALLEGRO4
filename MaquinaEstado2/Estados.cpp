#include "Global.h"

EstadoApresentacao* EstadoApresentacao::m_pInstancia = 0;

EstadoApresentacao* EstadoApresentacao::CriaInstancia()
{
  if(m_pInstancia == 0)
  {
    m_pInstancia = new EstadoApresentacao();
  }
  
  return m_pInstancia;
}

void EstadoApresentacao::AoEntrar()
{
}

void EstadoApresentacao::ExecutaFrame()
{
  printf("\nApresentacao\n");
}

void EstadoApresentacao::AoSair()
{
}

void EstadoApresentacao::AoTerminarTempo(Maquina* maquina)
{
  ExecutaTransicao(maquina, EstadoMenu::CriaInstancia());
}

EstadoMenu* EstadoMenu::m_pInstancia = 0;

EstadoMenu* EstadoMenu::CriaInstancia()
{
  if(m_pInstancia == 0)
  {
    m_pInstancia = new EstadoMenu();
  }
  
  return m_pInstancia;
}

void EstadoMenu::AoEntrar()
{
}

void EstadoMenu::ExecutaFrame()
{
  printf("\nMenu\n");
  printf("\n- Pressione 'j' para jogar");
  printf("\n- Pressione 'q' para sair\n\n");
}

void EstadoMenu::AoSair()
{
}

void EstadoMenu::AoPressionarSair(Maquina* maquina)
{
  maquina->Finalizar();
}

void EstadoMenu::AoPressionarJogar(Maquina* maquina)
{
  ExecutaTransicao(maquina, EstadoJogo::CriaInstancia());
}

EstadoJogo* EstadoJogo::m_pInstancia = 0;

EstadoJogo* EstadoJogo::CriaInstancia()
{
  if(m_pInstancia == 0)
  {
    m_pInstancia = new EstadoJogo();
  }
  
  return m_pInstancia;
}

void EstadoJogo::AoEntrar()
{
}

void EstadoJogo::ExecutaFrame()
{
  printf("\nJogo\n");
  printf("\nPressione 'ESC' para sair");
}

void EstadoJogo::AoSair()
{
}

void EstadoJogo::AoPressionarESC(Maquina* maquina)
{
  ExecutaTransicao(maquina, EstadoMenu::CriaInstancia());
}
