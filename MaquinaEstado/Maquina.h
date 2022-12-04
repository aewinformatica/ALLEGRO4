#ifndef MAQUINA_H
#define MAQUINA_H

class Estado;

class Maquina
{
  private:
    // Inst�ncia �nica da classe (Singleton)
    static Maquina* m_pInstancia;

    // Estado atual
    class Estado* m_pEstadoAtual;
    
    // Indica se a m�quina tem que parar
    bool m_Finalizar;
  public:
    // Construtor da classe
    Maquina();

    // Cria a inst�ncia �nica da classe (Singleton)
    static Maquina* CriaInstancia();

    // Executa um frame de anima��o do estado atual
    void ExecutaFrame();
    
    // Executa transi��o de estados
    void ExecutaTransicao(Estado* estado);
    
    // Finaliza a m�quina
    void Finalizar();
    
    // Pergunta se pode finalizar a m�quina
    bool PodeFinalizar();
    
    // Eventos de mudan�a de estado
    void AoPressionarJogar();
    void AoPressionarSair();
    void AoPressionarESC();
    void AoTerminarTempo();
};

#endif  
