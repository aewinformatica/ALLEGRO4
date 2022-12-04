#include "Global.h"

void Estado::ExecutaTransicao(Maquina* maquina, Estado* estado)
{
  maquina->ExecutaTransicao(estado);
}
