#ifndef ESTADOS_H
#define ESTADOS_H

class EstadoApresentacao : public Estado
{
  private:
    // Inst�ncia �nica da classe (Singleton)
    static EstadoApresentacao* m_pInstancia;
  public:
    // Cria a inst�ncia �nica da classe (Singleton)
    static EstadoApresentacao* CriaInstancia();

    void AoEntrar();

    void ExecutaFrame();

    void AoSair();
    
    // Eventos de mudan�a de estado
    void AoTerminarTempo(Maquina* maquina);
};

class EstadoMenu : public Estado
{
  private:
    // Inst�ncia �nica da classe (Singleton)
    static EstadoMenu* m_pInstancia;
  public:
    // Cria a inst�ncia �nica da classe (Singleton)
    static EstadoMenu* CriaInstancia();

    void AoEntrar();

    void ExecutaFrame();

    void AoSair();
    
    // Eventos de mudan�a de estado
    void AoPressionarJogar(Maquina* maquina);
    void AoPressionarSair(Maquina* maquina);
};

class EstadoJogo : public Estado
{
  private:
    // Inst�ncia �nica da classe (Singleton)
    static EstadoJogo* m_pInstancia;
  public:
    // Cria a inst�ncia �nica da classe (Singleton)
    static EstadoJogo* CriaInstancia();

    void AoEntrar();

    void ExecutaFrame();

    void AoSair();

    // Eventos de mudan�a de estado
    void AoPressionarESC(Maquina* maquina);
};

#endif
