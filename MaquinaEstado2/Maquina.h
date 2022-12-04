#ifndef MAQUINA_H
#define MAQUINA_H

class Estado;

class Maquina
{
  private:
    // Instância única da classe (Singleton)
    static Maquina* m_pInstancia;

    // Estado atual
    class Estado* m_pEstadoAtual;
    
    // Indica se a máquina tem que parar
    bool m_Finalizar;
  public:
    // Construtor da classe
    Maquina();

    // Cria a instância única da classe (Singleton)
    static Maquina* CriaInstancia();

    // Executa um frame de animação do estado atual
    void ExecutaFrame();
    
    // Executa transição de estados
    void ExecutaTransicao(Estado* estado);
    
    // Finaliza a máquina
    void Finalizar();
    
    // Pergunta se pode finalizar a máquina
    bool PodeFinalizar();
    
    // Eventos de mudança de estado
    void AoPressionarJogar();
    void AoPressionarSair();
    void AoPressionarESC();
    void AoTerminarTempo();
};

#endif  
