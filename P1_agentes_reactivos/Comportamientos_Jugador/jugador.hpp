#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <iostream>
#include <stdlib.h>
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      fil = 99;
      col = 99;
      brujula = 0;
      ultimaAccion = actIDLE;
      bien_situado = false;
      girar_derecha = false;
      va_a_por_objeto = false;
      proxima_accion = actIDLE;

      vector< unsigned char> aux(size, '?');
      for(unsigned int i = 0; i<size; i++)
        mi_mapa_auxiliar.push_back(aux);

      vector< unsigned char> aux2(size*10, '?');
      for(unsigned int i = 0; i<size*10; i++)
        mi_mapa_auxiliar_no_bien_situado.push_back(aux2);

      tiene_botas = false;
      tiene_llave = false;
      tiene_bikini = false;
      tiene_hueso = false;
      rellenar_mapa = true;
      tamanio_mapa = 100;
      va_a_por_PK = false;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);

    void Reinicio();

    void ActualizarMapaNorte(const vector< unsigned char> terreno);

    void ActualizarMapaEste(const vector< unsigned char> terreno);

    void ActualizarMapaSur(const vector< unsigned char> terreno);

    void ActualizarMapaOeste(const vector< unsigned char> terreno);

    Action Arbol(const vector< unsigned char> terreno, const unsigned char objeto);

    Action Agua(const vector< unsigned char> terreno, const unsigned char objeto);

    Action Precipicio(const vector< unsigned char> terreno);

    Action SueloPedregosoArenoso(const vector< unsigned char> terreno, const vector< unsigned char> superficie);

    Action PuntoReferencia(const vector< unsigned char> terreno);

    Action Muros(const vector< unsigned char> terreno);

    Action Puertas(const vector< unsigned char> terreno, const unsigned char objeto);

    bool HayPKCercaDerecha(const vector< unsigned char> terreno);

    bool HayPKCercaIzquierda(const vector< unsigned char> terreno);

    bool HayPKCercaRecto(const vector< unsigned char> terreno);

    bool HayObjetoCercaDerecha(const vector< unsigned char> superficie);

    bool HayObjetoCercaIzquierda(const vector< unsigned char> superficie);

    bool HayObjetoCercaRecto(const vector< unsigned char> superficie);

    int interact(Action accion, int valor);

    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    int fil;
    int col;
    int brujula;
    Action ultimaAccion;
    bool bien_situado;
    bool girar_derecha;
    bool va_a_por_objeto;
    Action proxima_accion;
    vector< std::vector< unsigned char> > mi_mapa_auxiliar;
    vector<std::vector<unsigned char>> mi_mapa_auxiliar_no_bien_situado;
    bool tiene_botas;
    bool tiene_llave;
    bool tiene_bikini;
    bool tiene_hueso;
    int num_objetos;
    bool rellenar_mapa;
    unsigned int tamanio_mapa;
    bool va_a_por_PK;
};


#endif
