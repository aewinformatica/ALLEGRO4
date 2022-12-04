#ifndef ESTADO_H
#define ESTADO_H

class Estado
{
  public:
    // Eventos de mudança de estado
    virtual void AoPressionarJogar(Maquina* maquina) {};
    virtual void AoPressionarSair(Maquina* maquina) {};
    virtual void AoPressionarESC(Maquina* maquina) {};
    virtual void AoTerminarTempo(Maquina* maquina) {};

    // Executa o evento de entrada do estado
    virtual void AoEntrar() {};

    // Executa um frame de animação do estado atual
    virtual void ExecutaFrame() {};

    // Executa o evento de saída do estado
    virtual void AoSair() {};
  protected:
    void ExecutaTransicao(Maquina* maquina, Estado* estado);
};

#endif  
