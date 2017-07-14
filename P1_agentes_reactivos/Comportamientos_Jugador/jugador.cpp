#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

Action ComportamientoJugador::think(Sensores sensores){

	// Actualizar variables de estado
	if(sensores.reset)
		Reinicio();

	if(rellenar_mapa){
		unsigned int i, j;

		// Rellenar todo el mapa con tierra
		for(i = 0; i < tamanio_mapa; ++i)
			for(j = 0; j < tamanio_mapa; ++j)
				mapaResultado[i][j] = 'T';

		//Rellenar parte de arriba
		for(i = 0; i < 3; ++i)
			for(j = 0; j < tamanio_mapa; ++j){
				mapaResultado[i][j] = 'P';
				mi_mapa_auxiliar[i][j] = 'P';
			}

		// Rellenar parte de abajo
		for(i = tamanio_mapa - 3; i < tamanio_mapa; ++i)
			for(j = 0; j < tamanio_mapa; ++j){
				mapaResultado[i][j] = 'P';
				mi_mapa_auxiliar[i][j] = 'P';
			}
		
		// Rellenar parte izquierda
		for(i = 0; i < tamanio_mapa; ++i)
			for(j = 0; j < 3; ++j){
				mapaResultado[i][j] = 'P';
				mi_mapa_auxiliar[i][j] = 'P';
			}

		// Rellenar parte derecha
		for(i = 0; i < tamanio_mapa; ++i)
			for(j = tamanio_mapa - 3; j < tamanio_mapa; ++j){
				mapaResultado[i][j] = 'P';
				mi_mapa_auxiliar[i][j] = 'P';
			}

		rellenar_mapa = false;
	}

	// Actualizar brújula o filas y columnas
	switch(ultimaAccion){
		case actFORWARD:
			// Si no hemos colisionado, actualizar filas o columnas
			if(!sensores.colision){
				switch(brujula){
					case 0: // Norte
						fil--;
						break;

					case 1: // Este
						col++;
						break;

					case 2: // Sur
						fil++;
						break;

					case 3: // Oeste
						col--;
						break;
				}
			}
			break;

		case actTURN_L:
			brujula = (brujula+3)%4;
			break;

		case actTURN_R:
			brujula = (brujula+1)%4;
			break;
	}

	// Si estamos sobre un PK, actualizar filas y columnas
	if(sensores.terreno[0] == 'K' && !bien_situado){
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
		bien_situado = true;
	}

	// Si estamos bien situados, actualizar
	if(bien_situado){
		mapaResultado[fil][col] = sensores.terreno[0];
		mi_mapa_auxiliar[fil][col] = sensores.terreno[0];

		switch(brujula){
			case 0:
				ActualizarMapaNorte(sensores.terreno);
				break;

			case 1:
				ActualizarMapaEste(sensores.terreno);
				break;

			case 2:
				ActualizarMapaSur(sensores.terreno);
				break;

			case 3:
				ActualizarMapaOeste(sensores.terreno);
				break;
		}
	}

	else
		mi_mapa_auxiliar_no_bien_situado[fil][col] = sensores.terreno[0];

	// Decidir acción
	Action accion;

	// Si va a por un objeto
	if(va_a_por_objeto){
		accion = proxima_accion;
		va_a_por_objeto = false;
	}

	// Si va a por un PK
	else if(va_a_por_PK){
		accion = proxima_accion;
		va_a_por_PK = false;
	}

	// Si se detecta un PK en la zona izquierda del vector terreno
	else if(HayPKCercaIzquierda(sensores.terreno) && !bien_situado){
		va_a_por_PK = true;
		accion = actFORWARD;
		proxima_accion = actTURN_L;
	}

	// Si se detecta un PK en la zona derecha del vector terreno
	else if(HayPKCercaDerecha(sensores.terreno) && !bien_situado){
		va_a_por_PK = true;
		accion = actFORWARD;
		proxima_accion = actTURN_R;
	}

	// Si hay algo en la superficie muy cerca
	else if(sensores.superficie[1] != '_' || sensores.superficie[2] != '_' || sensores.superficie[3] != '_'){

		// Si hay un objeto delante
		if(sensores.superficie[2] != '_' && sensores.superficie[2] != 'a' && sensores.superficie[2] != 'l' && sensores.superficie[2] != 'd'){

			// Si está en un bosque, en agua o tiene la mochila llena, esquiva el objeto
			if(sensores.terreno[0] == 'B' || sensores.terreno[0] == 'A')
				accion = actTURN_L;


			// Si no tiene ningún objeto, lo coge
			else if(num_objetos == 0){
				accion = actPICKUP;
				num_objetos++;
			}

			// Si tiene algún objeto, coge el objeto del suelo si aún no lo tiene en la mochila
			else{
				switch(sensores.superficie[2]){
					case '0':
						if(tiene_hueso)
							accion = actTURN_R;

						else{
							accion = actPUSH;
							tiene_hueso = true;
							num_objetos++;
							va_a_por_objeto = true;
							proxima_accion = actPICKUP;
						}
						break;

					case '1':
						if(tiene_bikini)
							accion = actTURN_R;
						
						else{
							accion = actPUSH;
							tiene_bikini = true;
							num_objetos++;
							va_a_por_objeto = true;
							proxima_accion = actPICKUP;
						}
						break;

					case '2':
						if(tiene_botas)
							accion = actTURN_R;
						
						else{
							accion = actPUSH;
							tiene_botas = true;
							num_objetos++;
							va_a_por_objeto = true;
							proxima_accion = actPICKUP;
						}
						break;

					case '3':
						if(tiene_llave)
							accion = actTURN_R;
						
						else{
							accion = actPUSH;
							tiene_llave = true;
							num_objetos++;
							va_a_por_objeto = true;
							proxima_accion = actPICKUP;
						}
						break;
				}
			}
		}

		// Si hay un objeto en el segundo elemento del vector superficie, ir hacia delante e indicar que va a por el objeto
		else if(sensores.superficie[1] != '_' && sensores.superficie[1] != 'a' && sensores.superficie[1] != 'l' && sensores.superficie[1] != 'd'){
			va_a_por_objeto = true;
			proxima_accion = actTURN_L;
			accion = actFORWARD;
		}

		// Si hay un objeto en el cuarto elemento del vector superficie, ir hacia delante e indicar que va a por el objeto
		else if(sensores.superficie[3] != '_' && sensores.superficie[3] != 'a' && sensores.superficie[3] != 'l' && sensores.superficie[3] != 'd'){
			va_a_por_objeto = true;
			proxima_accion = actTURN_R;
			accion = actFORWARD;
		}
		
		else if(sensores.superficie[2] == 'a')
			accion = actTURN_L;
		
		else if(sensores.superficie[2] == 'l'){
			if(sensores.objetoActivo == '0'){
				accion = actGIVE;
				num_objetos--;
			}

			else
				accion = actTURN_L;
		}

		else if(sensores.superficie[2] == 'd'){
			if(sensores.objetoActivo == '3'){
				accion = actGIVE;
				num_objetos--;
			}

			else
				accion = actTURN_L;
		}

		else{
			if(bien_situado){
				switch(brujula){
					case 0: // Norte
						if(mi_mapa_auxiliar[fil-1][col] == '?')
							accion = actFORWARD;

						else if(mi_mapa_auxiliar[fil][col-1] == '?')
							accion = actTURN_L;

						else if(mi_mapa_auxiliar[fil][col+1] == '?')
							accion = actTURN_R;

						else
							accion = actFORWARD;

						break;

					case 1: // Este
						if(mi_mapa_auxiliar[fil][col+1] == '?')
							accion = actFORWARD;

						else if(mi_mapa_auxiliar[fil+1][col] == '?')
							accion = actTURN_R;

						else if(mi_mapa_auxiliar[fil-1][col] == '?')
							accion = actTURN_L;

						else
							accion = actFORWARD;

						break;

					case 2: // Sur
						if(mi_mapa_auxiliar[fil+1][col] == '?')
							accion = actFORWARD;

						else if(mi_mapa_auxiliar[fil][col+1] == '?')
							accion = actTURN_L;

						else if(mi_mapa_auxiliar[fil][col-1] == '?')
							accion = actTURN_R;

						else
							accion = actFORWARD;

						break;

					case 3: // Oeste
						if(mi_mapa_auxiliar[fil][col-1] == '?')
							accion = actFORWARD;

						else if(mi_mapa_auxiliar[fil-1][col] == '?')
							accion = actTURN_R;

						else if(mi_mapa_auxiliar[fil+1][col] == '?')
							accion = actTURN_L;

						else
							accion = actFORWARD;

						break;
				}	/// Switch		
			} // if

			else{
				if (!girar_derecha){
					accion = actTURN_L;
					if (rand()%2==0) girar_derecha = true;
					else girar_derecha = false;
				}

				else{
					accion = actTURN_R;
					if (rand()%2==0) girar_derecha = true;
					else girar_derecha = false;
				}
			}
		}
	}

	else{
		switch(sensores.terreno[2]){
			case 'B':
				accion = Arbol(sensores.terreno, sensores.objetoActivo);
				break;

			case 'A':
				accion = Agua(sensores.terreno, sensores.objetoActivo);
				break;

			case 'P':
				accion = Precipicio(sensores.terreno);
				break;

			case 'K':
				accion = PuntoReferencia(sensores.terreno);
				break;

			case 'M':
				accion = Muros(sensores.terreno);
				break;

			case 'D':
				accion = Puertas(sensores.terreno, sensores.objetoActivo);
				break;

			default:
				accion = SueloPedregosoArenoso(sensores.terreno, sensores.superficie);
				break;
		}
	}

	ultimaAccion = accion;

	// Imprimir variables

	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Mochila: " << sensores.mochila << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << "Fila: " << sensores.mensajeF << endl;
	cout << "Columna: " << sensores.mensajeC << endl;
	cout << "objetoActivo: " << sensores.objetoActivo << endl;

	cout << endl;

	if(!bien_situado){
		switch(brujula){
			case 0:	// Norte
				mi_mapa_auxiliar_no_bien_situado[fil-1][col] = sensores.terreno[2];

				if(sensores.terreno[1] == 'M')
					mi_mapa_auxiliar_no_bien_situado[fil-1][col-1] = sensores.terreno[1];

				if(sensores.terreno[3] == 'M')
					mi_mapa_auxiliar_no_bien_situado[fil-1][col+1] = sensores.terreno[3];
				break;

			case 1:	// Este
				mi_mapa_auxiliar_no_bien_situado[fil][col+1] = sensores.terreno[2];

				if(sensores.terreno[1] == 'M')
					mi_mapa_auxiliar_no_bien_situado[fil-1][col+1] = sensores.terreno[1];

				if(sensores.terreno[3] == 'M')
					mi_mapa_auxiliar_no_bien_situado[fil+1][col+1] = sensores.terreno[3];
				break;

			case 2:	// Sur
				mi_mapa_auxiliar_no_bien_situado[fil+1][col] = sensores.terreno[2];

				if(sensores.terreno[1] == 'M')
					mi_mapa_auxiliar_no_bien_situado[fil+1][col+1] = sensores.terreno[1];

				if(sensores.terreno[3] == 'M')
					mi_mapa_auxiliar_no_bien_situado[fil+1][col-1] = sensores.terreno[3];			
				break;

			case 3:	// Oeste
				mi_mapa_auxiliar_no_bien_situado[fil][col-1] = sensores.terreno[2];

				if(sensores.terreno[1] == 'M')
					mi_mapa_auxiliar_no_bien_situado[fil+1][col-1] = sensores.terreno[1];

				if(sensores.terreno[3] == 'M')
					mi_mapa_auxiliar_no_bien_situado[fil-1][col-1] = sensores.terreno[3];
				break;
		}
	}

	return accion;
}

void ComportamientoJugador::ActualizarMapaNorte(const vector< unsigned char> terreno){
	mapaResultado[fil-1][col-1] = terreno[1];
	mapaResultado[fil-1][col] = terreno[2];
	mapaResultado[fil-1][col+1] = terreno[3];
	mapaResultado[fil-2][col-2] = terreno[4];
	mapaResultado[fil-2][col-1] = terreno[5];
	mapaResultado[fil-2][col] = terreno[6];
	mapaResultado[fil-2][col+1] = terreno[7];
	mapaResultado[fil-2][col+2] = terreno[8];
	mapaResultado[fil-3][col-3] = terreno[9];
	mapaResultado[fil-3][col-2] = terreno[10];
	mapaResultado[fil-3][col-1] = terreno[11];
	mapaResultado[fil-3][col] = terreno[12];
	mapaResultado[fil-3][col+1] = terreno[13];
	mapaResultado[fil-3][col+2] = terreno[14];
	mapaResultado[fil-3][col+3] = terreno[15];

	mi_mapa_auxiliar[fil-1][col] = terreno[2];

	if(terreno[1] == 'M')
		mi_mapa_auxiliar[fil-1][col-1] = terreno[1];

	if(terreno[3] == 'M')
		mi_mapa_auxiliar[fil-1][col+1] = terreno[3];
}

void ComportamientoJugador::ActualizarMapaEste(const vector< unsigned char> terreno){
	mapaResultado[fil-1][col+1] = terreno[1];
	mapaResultado[fil][col+1] = terreno[2];
	mapaResultado[fil+1][col+1] = terreno[3];
	mapaResultado[fil-2][col+2] = terreno[4];
	mapaResultado[fil-1][col+2] = terreno[5];
	mapaResultado[fil][col+2] = terreno[6];
	mapaResultado[fil+1][col+2] = terreno[7];
	mapaResultado[fil+2][col+2] = terreno[8];
	mapaResultado[fil-3][col+3] = terreno[9];
	mapaResultado[fil-2][col+3] = terreno[10];
	mapaResultado[fil-1][col+3] = terreno[11];
	mapaResultado[fil][col+3] = terreno[12];
	mapaResultado[fil+1][col+3] = terreno[13];
	mapaResultado[fil+2][col+3] = terreno[14];
	mapaResultado[fil+3][col+3] = terreno[15];

	mi_mapa_auxiliar[fil][col+1] = terreno[2];

	if(terreno[1] == 'M')
		mi_mapa_auxiliar[fil-1][col+1] = terreno[1];

	if(terreno[3] == 'M')
		mi_mapa_auxiliar[fil+1][col+1] = terreno[3];
}

void ComportamientoJugador::ActualizarMapaSur(const vector< unsigned char> terreno){
	mapaResultado[fil+1][col+1] = terreno[1];
	mapaResultado[fil+1][col] = terreno[2];
	mapaResultado[fil+1][col-1] = terreno[3];
	mapaResultado[fil+2][col+2] = terreno[4];
	mapaResultado[fil+2][col+1] = terreno[5];
	mapaResultado[fil+2][col] = terreno[6];
	mapaResultado[fil+2][col-1] = terreno[7];
	mapaResultado[fil+2][col-2] = terreno[8];
	mapaResultado[fil+3][col+3] = terreno[9];
	mapaResultado[fil+3][col+2] = terreno[10];
	mapaResultado[fil+3][col+1] = terreno[11];
	mapaResultado[fil+3][col] = terreno[12];
	mapaResultado[fil+3][col-1] = terreno[13];
	mapaResultado[fil+3][col-2] = terreno[14];
	mapaResultado[fil+3][col-3] = terreno[15];

	mi_mapa_auxiliar[fil+1][col] = terreno[2];

	if(terreno[1] == 'M')
		mi_mapa_auxiliar[fil+1][col+1] = terreno[1];

	if(terreno[3] == 'M')
		mi_mapa_auxiliar[fil+1][col-1] = terreno[3];
}

void ComportamientoJugador::ActualizarMapaOeste(const vector< unsigned char> terreno){
	mapaResultado[fil+1][col-1] = terreno[1];
	mapaResultado[fil][col-1] = terreno[2];
	mapaResultado[fil-1][col-1] = terreno[3];
	mapaResultado[fil+2][col-2] = terreno[4];
	mapaResultado[fil+1][col-2] = terreno[5];
	mapaResultado[fil][col-2] = terreno[6];
	mapaResultado[fil-1][col-2] = terreno[7];
	mapaResultado[fil-2][col-2] = terreno[8];
	mapaResultado[fil+3][col-3] = terreno[9];
	mapaResultado[fil+2][col-3] = terreno[10];
	mapaResultado[fil+1][col-3] = terreno[11];
	mapaResultado[fil][col-3] = terreno[12];
	mapaResultado[fil-1][col-3] = terreno[13];
	mapaResultado[fil-2][col-3] = terreno[14];
	mapaResultado[fil-3][col-3] = terreno[15];


	mi_mapa_auxiliar[fil][col-1] = terreno[2];
	if(terreno[1] == 'M')
		mi_mapa_auxiliar[fil+1][col-1] = terreno[1];

	if(terreno[3] == 'M')
		mi_mapa_auxiliar[fil-1][col-1] = terreno[3];
}

Action ComportamientoJugador::Arbol(const vector< unsigned char> terreno, const unsigned char objeto){
	Action accion;

	if(bien_situado){ 
		if(objeto == '2'){
			switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 1: // Este
					if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;
				}
		}

		else if(tiene_botas && terreno[0] != 'A'){
			if(ultimaAccion == actPUSH)
				accion = actPOP;

			else
				accion = actPUSH;
		}

		else{
			switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar[fil-1][col] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_R;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;

				case 1: // Este
					if(mi_mapa_auxiliar[fil][col+1] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_L;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar[fil+1][col] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_R;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar[fil][col-1] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_L;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;
			}	// Switch
		}
	}
	
	else{
		if(objeto == '2'){
			switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 1: // Este
					if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;
				}
		}

		else if(tiene_botas && terreno[0] != 'A'){
			if(ultimaAccion == actPUSH)
				accion = actPOP;

			else
				accion = actPUSH;
		}

		else{
			switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_R;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;

				case 1: // Este
					if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_L;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_R;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_L;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;
			}	// Switch
		}
	}

	return accion;
}

Action ComportamientoJugador::Agua(const vector< unsigned char> terreno, const unsigned char objeto){
	Action accion;

	if(bien_situado){
		if(objeto == '1'){
			switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 1: // Este
					if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;
				}
		}

		else if(tiene_bikini && terreno[0] != 'B'){
			if(ultimaAccion == actPUSH)
				accion = actPOP;

			else
				accion = actPUSH;
		}

		else{
			switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar[fil-1][col] == '?'){
						if(terreno[2] == 'A')
							accion = actTURN_L;
						else
							accion = actFORWARD;
					}

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_R;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
						
					}

					break;

				case 1: // Este
					if(mi_mapa_auxiliar[fil][col+1] == '?'){
						if(terreno[2] == 'A')
							accion = actTURN_L;
						else
							accion = actFORWARD;
					}

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_L;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
						
					}

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar[fil+1][col] == '?'){
						if(terreno[2] == 'A')
							accion = actTURN_L;
						else
							accion = actFORWARD;
					}

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_R;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar[fil][col-1] == '?'){
						if(terreno[2] == 'A')
							accion = actTURN_L;
						else
							accion = actFORWARD;
					}

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_L;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;
			}	// Switch
		}
	}

	else{
		if(objeto == '1'){
			switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 1: // Este
					if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;
				}
		}

		else if(tiene_bikini && terreno[0] != 'B'){
			if(ultimaAccion == actPUSH)
				accion = actPOP;

			else
				accion = actPUSH;
		}

		else{
			switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?'){
						if(terreno[2] == 'A')
							accion = actTURN_L;
						else
							accion = actFORWARD;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_R;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
						
					}

					break;

				case 1: // Este
					if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?'){
						if(terreno[2] == 'A')
							accion = actTURN_L;
						else
							accion = actFORWARD;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_L;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
						
					}

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?'){
						if(terreno[2] == 'A')
							accion = actTURN_L;
						else
							accion = actFORWARD;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_R;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?'){
						if(terreno[2] == 'A')
							accion = actTURN_L;
						else
							accion = actFORWARD;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_L;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}

					break;
			}	// Switch
		}
	}

	return accion;
}

Action ComportamientoJugador::Precipicio(const vector< unsigned char> terreno){
	Action accion;

	if(bien_situado){
		switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar[fil-1][col] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_R;

					else{
						if(mi_mapa_auxiliar[fil][col-2] == '?')
							accion = actTURN_L;

						else if(mi_mapa_auxiliar[fil][col+2] == '?')
							accion = actTURN_R;

						else{
							if (!girar_derecha){
								accion = actTURN_L;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}

							else{
								accion = actTURN_R;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}
						}
					}

					break;

				case 1: // Este
					if(mi_mapa_auxiliar[fil][col+1] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_L;

					else{
						if(mi_mapa_auxiliar[fil+2][col] == '?')
							accion = actTURN_R;

						else if(mi_mapa_auxiliar[fil-2][col] == '?')
							accion = actTURN_L;

						else{
							if (!girar_derecha){
								accion = actTURN_L;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}

							else{
								accion = actTURN_R;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}
						}
					}

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar[fil+1][col] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_R;

					else{
						if(mi_mapa_auxiliar[fil][col+2] == '?')
							accion = actTURN_L;

						else if(mi_mapa_auxiliar[fil][col-2] == '?')
							accion = actTURN_R;

						else{
							if (!girar_derecha){
								accion = actTURN_L;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}

							else{
								accion = actTURN_R;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}
						}
					}

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar[fil][col-1] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_L;

					else{
						if(mi_mapa_auxiliar[fil-2][col] == '?')
							accion = actTURN_R;

						else if(mi_mapa_auxiliar[fil+2][col] == '?')
							accion = actTURN_L;

						else{
							if (!girar_derecha){
								accion = actTURN_L;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}

							else{
								accion = actTURN_R;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}
						}
					}

					break;
			}	// Switch
	}

	else{
		switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_R;

					else{
							if (!girar_derecha){
								accion = actTURN_L;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}

							else{
								accion = actTURN_R;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}
					}

					break;

				case 1: // Este
					if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_L;

					else{

							if (!girar_derecha){
								accion = actTURN_L;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}

							else{
								accion = actTURN_R;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}
					}

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_R;

					else{

							if (!girar_derecha){
								accion = actTURN_L;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}

							else{
								accion = actTURN_R;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}
						
					}

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?'){
						accion = actTURN_L;
					}

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_L;

					else{

							if (!girar_derecha){
								accion = actTURN_L;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}

							else{
								accion = actTURN_R;
								if (rand()%2==0) girar_derecha = true;
								else girar_derecha = false;
							}
						
					}

					break;
			}	// Switch
	}
	
	return accion;
}

Action ComportamientoJugador::SueloPedregosoArenoso(const vector< unsigned char> terreno,  const vector< unsigned char> superficie){
	Action accion;

	// Si no ha llegado a un PK todavía
	if(!bien_situado){
		if(HayPKCercaRecto(terreno) || HayPKCercaDerecha(terreno) || HayPKCercaIzquierda(terreno)){
			accion = actFORWARD;
		}

		else{
			switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 1: // Este
					if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;
				}
		}
	}

	// Si ya ha pasado por un PK
	else{
		// Si hay un objeto cerca, ir a por él
		if(HayObjetoCercaRecto(superficie) || HayObjetoCercaDerecha(superficie) || HayObjetoCercaIzquierda(superficie)){
				accion = actFORWARD;
			}
		
		// Si no hay un objeto cerca, ir hacia una casilla desconocida aún
		else{
			switch(brujula){
				case 0: // Norte
					if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 1: // Este
					if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;

				case 2: // Sur
					if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil][col+1] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actTURN_R;

					else
						accion = actFORWARD;

					break;

				case 3: // Oeste
					if(mi_mapa_auxiliar[fil][col-1] == '?')
						accion = actFORWARD;

					else if(mi_mapa_auxiliar[fil-1][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil+1][col] == '?')
						accion = actTURN_L;

					else
						accion = actFORWARD;

					break;
			}
		}		
	}

	return accion;
}

Action ComportamientoJugador::PuntoReferencia(const vector< unsigned char> terreno){
	return actFORWARD;
}

Action ComportamientoJugador::Muros(const vector< unsigned char> terreno){
	Action accion;

	if(bien_situado){
		switch(brujula){
			case 0: // Norte
				if(mi_mapa_auxiliar[fil-1][col] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar[fil][col-1] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar[fil][col+1] == '?')
					accion = actTURN_R;

				else{
					if(mi_mapa_auxiliar[fil][col-2] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col+2] == '?')
						accion = actTURN_R;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}
				}

				break;

			case 1: // Este
				if(mi_mapa_auxiliar[fil][col+1] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar[fil+1][col] == '?')
					accion = actTURN_R;

				else if(mi_mapa_auxiliar[fil-1][col] == '?')
					accion = actTURN_L;

				else{
					if(mi_mapa_auxiliar[fil+2][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil-2][col] == '?')
						accion = actTURN_L;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}
				}

				break;

			case 2: // Sur
				if(mi_mapa_auxiliar[fil+1][col] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar[fil][col+1] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar[fil][col-1] == '?')
					accion = actTURN_R;

				else{
					if(mi_mapa_auxiliar[fil][col+2] == '?')
						accion = actTURN_L;

					else if(mi_mapa_auxiliar[fil][col-2] == '?')
						accion = actTURN_R;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}
				}

				break;

			case 3: // Oeste
				if(mi_mapa_auxiliar[fil][col-1] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar[fil-1][col] == '?')
					accion = actTURN_R;

				else if(mi_mapa_auxiliar[fil+1][col] == '?')
					accion = actTURN_L;

				else{
					if(mi_mapa_auxiliar[fil-2][col] == '?')
						accion = actTURN_R;

					else if(mi_mapa_auxiliar[fil+2][col] == '?')
						accion = actTURN_L;

					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}
				}

				break;
		}	// Switch
	}

	else{
		switch(brujula){
			case 0: // Norte
				if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
					accion = actTURN_R;


					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}
			

				break;

			case 1: // Este
				if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
					accion = actTURN_R;

				else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
					accion = actTURN_L;


					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}
			

				break;

			case 2: // Sur
				if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar_no_bien_situado[fil][col+1] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
					accion = actTURN_R;


					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}
				

				break;

			case 3: // Oeste
				if(mi_mapa_auxiliar_no_bien_situado[fil][col-1] == '?')
					accion = actTURN_L;

				else if(mi_mapa_auxiliar_no_bien_situado[fil-1][col] == '?')
					accion = actTURN_R;

				else if(mi_mapa_auxiliar_no_bien_situado[fil+1][col] == '?')
					accion = actTURN_L;


					else{
						if (!girar_derecha){
							accion = actTURN_L;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}

						else{
							accion = actTURN_R;
							if (rand()%2==0) girar_derecha = true;
							else girar_derecha = false;
						}
					}
				

				break;
		}	// Switch
	}

	return accion;
}

Action ComportamientoJugador::Puertas(const vector< unsigned char> terreno, const unsigned char objeto){
	Action accion;

	if(objeto == '3')
		accion = actFORWARD;

	else if(tiene_llave){
		if(ultimaAccion == actPUSH)
			accion = actPOP;

		else
			accion = actPUSH;
	}

	else{
		if (!girar_derecha){
			accion = actTURN_L;
			if (rand()%2==0) girar_derecha = true;
			else girar_derecha = false;
		}

		else{
			accion = actTURN_R;
			if (rand()%2==0) girar_derecha = true;
			else girar_derecha = false;
		}
	}

	return accion;
}

bool ComportamientoJugador::HayPKCercaDerecha(const vector< unsigned char> terreno){
	if(terreno[3] == 'K' || terreno[7] == 'K' || terreno[8] == 'K' || terreno[13] == 'K' || terreno[14] == 'K' || terreno[15] == 'K')
		return true;

	return false;
}

bool ComportamientoJugador::HayPKCercaIzquierda(const vector< unsigned char> terreno){
	if(terreno[1] == 'K' || terreno[4] == 'K' || terreno[5] == 'K' || terreno[9] == 'K' || terreno[10] == 'K' || terreno[11] == 'K')
		return true;

	return false;
}

bool ComportamientoJugador::HayPKCercaRecto(const vector< unsigned char> terreno){
	if(terreno[6] == 'K' || terreno[12] == 'K')
		return true;

	return false;
}

bool ComportamientoJugador::HayObjetoCercaDerecha(const vector< unsigned char> superficie){
	if(!tiene_hueso)
		if(superficie[3] == '0' || superficie[7] == '0' || superficie[8] == '0' || superficie[13] == '0' || superficie[14] == '0' || superficie[15] == '0')
			return true;

	if(!tiene_bikini)
		if(superficie[3] == '1' || superficie[7] == '1' || superficie[8] == '1' || superficie[13] == '1' || superficie[14] == '1' || superficie[15] == '1')
			return true;

	if(!tiene_botas)
		if(superficie[3] == '2' || superficie[7] == '2' || superficie[8] == '2' || superficie[13] == '2' || superficie[14] == '2' || superficie[15] == '2')
			return true;

	if(!tiene_llave)
		if(superficie[3] == '3' || superficie[7] == '3' || superficie[8] == '3' || superficie[13] == '3' || superficie[14] == '3' || superficie[15] == '3')
			return true;

	return false;
}

bool ComportamientoJugador::HayObjetoCercaIzquierda(const vector< unsigned char> superficie){
	if(!tiene_hueso)
		if(superficie[1] == '0' || superficie[4] == '0' || superficie[5] == '0' || superficie[9] == '0' || superficie[10] == '0' || superficie[11] == '0')
			return true;

	if(!tiene_bikini)
		if(superficie[1] == '1' || superficie[4] == '1' || superficie[5] == '1' || superficie[9] == '1' || superficie[10] == '1' || superficie[11] == '1')
			return true;

	if(!tiene_botas)
		if(superficie[1] == '2' || superficie[4] == '2' || superficie[5] == '2' || superficie[9] == '2' || superficie[10] == '2' || superficie[1] == '2')
			return true;

	if(!tiene_llave)
		if(superficie[1] == '3' || superficie[4] == '3' || superficie[5] == '3' || superficie[9] == '3' || superficie[10] == '3' || superficie[1] == '3')
			return true;

	return false;
}

bool ComportamientoJugador::HayObjetoCercaRecto(const vector< unsigned char> superficie){
	if(!tiene_hueso)
		if(superficie[6] == '0' || superficie[12] == '0')
			return true;

	if(!tiene_bikini)
		if(superficie[6] == '1' || superficie[12] == '1')
			return true;

	if(!tiene_botas)
		if(superficie[6] == '2' || superficie[12] == '2')
			return true;

	if(!tiene_llave)
		if(superficie[6] == '3' || superficie[12] == '3')
			return true;

	return false;
}

void ComportamientoJugador::Reinicio(){
	fil = 99;
   col = 99;
   brujula = 0;
   ultimaAccion = actIDLE;
   bien_situado = false;
   girar_derecha = false;
   va_a_por_objeto = false;
   proxima_accion = actIDLE;
   tiene_botas = false;
   tiene_llave = false;
   tiene_bikini = false;
   tiene_hueso = false;
   num_objetos = 0;
   va_a_por_PK = false;
   rellenar_mapa = false;

   vector< unsigned char> aux(tamanio_mapa*10, '?');
   mi_mapa_auxiliar_no_bien_situado.clear();
   for(unsigned int i = 0; i<tamanio_mapa*10; i++)
      mi_mapa_auxiliar_no_bien_situado.push_back(aux);
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
