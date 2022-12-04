#include "Global.h"

Maquina::Maquina()
{
  m_pEstadoAtual = 0;
  m_Finalizar = false;
}

// Definição do atributo instância
Maquina* Maquina::m_pInstancia = 0;

// Cria a instância única da classe (Singleton)
Maquina* Maquina::CriaInstancia()
{
  if(m_pInstancia == 0)
  {
    m_pInstancia = new Maquina();
  }
  
  return m_pInstancia;
}

// Executa uma transição de estado
void Maquina::ExecutaTransicao(Estado* estado)
{
  // Executa o evento AoSair() do estado antigo antes de executar a transição
  if(m_pEstadoAtual != 0)
  {
    m_pEstadoAtual->AoSair();
  }

  m_pEstadoAtual = estado;
  
  // Executa o evento AoEntrar() do estado novo logo após executar a transição
  m_pEstadoAtual->AoEntrar();
}

// Executa um frame de animação do estado atual
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

// Eventos de mudança de estado
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
