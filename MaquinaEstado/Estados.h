#ifndef ESTADOS_H
#define ESTADOS_H

class EstadoApresentacao : public Estado
{
  private:
    // Instância única da classe (Singleton)
    static EstadoApresentacao* m_pInstancia;
  public:
    // Cria a instância única da classe (Singleton)
    static EstadoApresentacao* CriaInstancia();

    void AoEntrar();

    void ExecutaFrame();

    void AoSair();
    
    // Eventos de mudança de estado
    void AoTerminarTempo(Maquina* maquina);
};

class EstadoMenu : public Estado
{
  private:
    // Instância única da classe (Singleton)
    static EstadoMenu* m_pInstancia;
  public:
    // Cria a instância única da classe (Singleton)
    static EstadoMenu* CriaInstancia();

    void AoEntrar();

    void ExecutaFrame();

    void AoSair();
    
    // Eventos de mudança de estado
    void AoPressionarJogar(Maquina* maquina);
    void AoPressionarSair(Maquina* maquina);
};

class EstadoJogo : public Estado
{
  private:
    // Instância única da classe (Singleton)
    static EstadoJogo* m_pInstancia;
  public:
    // Cria a instância única da classe (Singleton)
    static EstadoJogo* CriaInstancia();

    void AoEntrar();

    void ExecutaFrame();

    void AoSair();

    // Eventos de mudança de estado
    void AoPressionarESC(Maquina* maquina);
};

#endif
