#include "Global.h"

Maquina::Maquina()
{
  m_pEstadoAtual = 0;
  m_Finalizar = false;
}

// Defini��o do atributo inst�ncia
Maquina* Maquina::m_pInstancia = 0;

// Cria a inst�ncia �nica da classe (Singleton)
Maquina* Maquina::CriaInstancia()
{
  if(m_pInstancia == 0)
  {
    m_pInstancia = new Maquina();
  }
  
  return m_pInstancia;
}

// Executa uma transi��o de estado
void Maquina::ExecutaTransicao(Estado* estado)
{
  // Executa o evento AoSair() do estado antigo antes de executar a transi��o
  if(m_pEstadoAtual != 0)
  {
    m_pEstadoAtual->AoSair();
  }

  m_pEstadoAtual = estado;
  
  // Executa o evento AoEntrar() do estado novo logo ap�s executar a transi��o
  m_pEstadoAtual->AoEntrar();
}

// Executa um frame de anima��o do estado atual
void Maquina::ExecutaFrame()
{
  m_pEstadoAtual->ExecutaFrame();
}

void Maquina::Finalizar()
{
  m_Finalizar = true;
}

bool Maquina::PodeFinalizar()
{
  return m_Finalizar;
}

// Eventos de mudan�a de estado
void Maquina::AoPressionarJogar()
{
  m_pEstadoAtual->AoPressionarJogar(this);
}

void Maquina::AoPressionarSair()
{
  m_pEstadoAtual->AoPressionarSair(this);
}

void Maquina::AoPressionarESC()
{
  m_pEstadoAtual->AoPressionarESC(this);
}

void Maquina::AoTerminarTempo()
{
  m_pEstadoAtual->AoTerminarTempo(this);
}
