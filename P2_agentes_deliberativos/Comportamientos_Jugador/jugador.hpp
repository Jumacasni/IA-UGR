#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <iostream>
#include <stdlib.h>
#include <list>
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

      // Variables práctica 2

      estoy_ejecutando_plan = false;
      error_plan = false;
      tiene_regalo = false;
      num_turnos = 0;
      rey_localizado = false;
      va_a_por_rey = false;
      replanificar = false;
    }

    struct estado{
      int fila;
      int columna;
      int orientacion;

      bool operator!=(const estado & st) const{
        if(fila != st.fila || columna != st.columna)
          return true;

        return false;
      }

      bool operator==(const estado & st) const{
        if(fila == st.fila && columna == st.columna)
          return true;

        return false;
      }

      estado & operator=(const estado & st){
        fila = st.fila;
        columna = st.columna;
        orientacion = st.orientacion;

        return *this;
      }
    };

    struct nodo{
      estado state;
      list<Action> acciones;

      bool operator==(const nodo & st) const{
        if(state.fila == st.state.fila && state.columna == st.state.columna && state.orientacion == st.state.orientacion)
          return true;

        return false;
      }

      bool operator!=(const nodo & st) const{
        if(state.fila != st.state.fila || state.columna != st.state.columna || state.orientacion != st.state.orientacion)
          return true;

        return false;
      }

      nodo & operator=(const nodo & g){
        state = g.state;
        acciones = g.acciones;

        return *this;
      }
    };

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);

    void ActualizarMapaNorte(const vector< unsigned char> terreno);

    void ActualizarMapaEste(const vector< unsigned char> terreno);

    void ActualizarMapaSur(const vector< unsigned char> terreno);

    void ActualizarMapaOeste(const vector< unsigned char> terreno);

    void ActualizarLocalizacionReyNorte(const vector< unsigned char> superficie, estado & rey);

    void ActualizarLocalizacionReyEste(const vector< unsigned char> superficie, estado & rey);

    void ActualizarLocalizacionReySur(const vector< unsigned char> superficie, estado & rey);

    void ActualizarLocalizacionReyOeste(const vector< unsigned char> superficie, estado & rey);

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

    bool HayReyCercaDerecha(const vector< unsigned char> superficie);

    bool HayReyCercaIzquierda(const vector< unsigned char> superficie);

    bool HayReyCercaRecto(const vector< unsigned char> superficie);

    bool HayObjetoCercaDerecha(const vector< unsigned char> superficie);

    bool HayObjetoCercaIzquierda(const vector< unsigned char> superficie);

    bool HayObjetoCercaRecto(const vector< unsigned char> superficie);

    bool HayReyCerca(const vector< unsigned char> superficie);

    void PintaPlan(list<Action> plan);

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

    // Nuevas variables de la pŕáctica 2

    bool tiene_regalo;
    bool estoy_ejecutando_plan;
    bool error_plan;
    list<Action> plan;
    list<estado> reyes;

    void Reinicio();
    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);
    bool find_nodo(const list<nodo> & l, const nodo & n);
    bool find_estado(const list<estado> & l, const estado & r);
    int num_turnos;
    bool rey_localizado;
    bool va_a_por_rey;
    bool replanificar;
};


#endif
