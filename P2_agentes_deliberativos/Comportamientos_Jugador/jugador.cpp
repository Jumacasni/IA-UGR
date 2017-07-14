#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <math.h>

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

	// Control del plan
	if (error_plan){
		estoy_ejecutando_plan = false;
		num_turnos = 0;
	}

	// Monitorización del plan
	if (estoy_ejecutando_plan && plan.front() == actFORWARD){
		if (sensores.superficie[2] == 'a' || sensores.superficie[2] == 'l'){
			plan.push_front(actIDLE);
			error_plan = false;
		}

		else if(sensores.superficie[2] == '0' || sensores.superficie[2] == '1' || sensores.superficie[2] == '2' || sensores.superficie[2] == '3' || sensores.superficie[2] == '4'){
				switch(sensores.superficie[2]){
					case '0':
						plan.push_front(actTURN_L);
						plan.push_front(actTHROW);
						plan.push_front(actTURN_R);
						plan.push_front(actPICKUP);
						plan.push_front(actPUSH);
						break;

					case '1':
						if(tiene_bikini){
							plan.push_front(actTURN_L);
							plan.push_front(actTHROW);
							plan.push_front(actTURN_R);
							plan.push_front(actPICKUP);
							plan.push_front(actPUSH);
						}
							
						else{
							plan.push_front(actPICKUP);
							plan.push_front(actPUSH);
						}
						break;

					case '2':
						if(tiene_botas){
							plan.push_front(actTURN_L);
							plan.push_front(actTHROW);
							plan.push_front(actTURN_R);
							plan.push_front(actPICKUP);
							plan.push_front(actPUSH);
						}
						
						else{
							plan.push_front(actPICKUP);
							plan.push_front(actPUSH);
						}
						break;

					case '3':
						if(tiene_llave){
							plan.push_front(actTURN_L);
							plan.push_front(actTHROW);
							plan.push_front(actTURN_R);
							plan.push_front(actPICKUP);
							plan.push_front(actPUSH);
						}

						else{
							plan.push_front(actPICKUP);
							plan.push_front(actPUSH);
						}

						break;

					case '4':
						if(tiene_regalo){
							plan.push_front(actTURN_L);
							plan.push_front(actTHROW);
							plan.push_front(actTURN_R);
							plan.push_front(actPICKUP);
							plan.push_front(actPUSH);
						}
						
						else{
							plan.push_front(actPICKUP);
							plan.push_front(actPUSH);
						}
						break;
				}

				error_plan = false;
		}

		else if(sensores.terreno[2] == 'A' && tiene_bikini && sensores.terreno[0] != 'B'){
			if(sensores.objetoActivo != '1'){
				plan.push_front(actPOP);
				plan.push_front(actPUSH);
				error_plan = false;
			}
		}

		else if(sensores.terreno[2] == 'B' && tiene_botas && sensores.terreno[0] != 'A'){
			if(sensores.objetoActivo != '2'){
				plan.push_front(actPOP);
				plan.push_front(actPUSH);
				error_plan = false;
			}
		}

		else if(sensores.terreno[2] == 'A' && !tiene_bikini){
			error_plan = true;
		}

		else if(sensores.terreno[2] == 'B' && !tiene_botas){
			error_plan = true;
		}

		else if(sensores.terreno[2] == 'P' || sensores.terreno[2] == 'M'){
			error_plan = true;
		}

		else
			error_plan = false;
	}

	else {
		error_plan = false;
	}

	if(bien_situado && HayReyCerca(sensores.superficie)){
		rey_localizado = true;
		estado rey;

		switch(brujula){
			case 0:
				ActualizarLocalizacionReyNorte(sensores.superficie, rey);
				break;

			case 1:
				ActualizarLocalizacionReyEste(sensores.superficie, rey);
				break;

			case 2:
				ActualizarLocalizacionReySur(sensores.superficie, rey);
				break;

			case 3:
				ActualizarLocalizacionReyOeste(sensores.superficie, rey);
				break;
		}

		if(!find_estado(reyes, rey) && mi_mapa_auxiliar[rey.fila][rey.columna] != 'A' && mi_mapa_auxiliar[rey.fila][rey.columna] != 'B'){
			reyes.push_back(rey);
			estoy_ejecutando_plan = false;
			error_plan = true;
			replanificar = true;
		}
	}
	
	// Planificación
	if (bien_situado && !tiene_regalo && !estoy_ejecutando_plan && sensores.regalos.size()>0 && !reyes.empty() && num_turnos > 200){
		estado origen;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		estado destino;
		destino.fila = sensores.regalos[0].first;
		destino.columna = sensores.regalos[0].second;

		double modulo_fila = pow((destino.fila - origen.fila), 2);
		double modulo_columna = pow((destino.columna - origen.columna), 2);
		double mejor_distancia = sqrt(modulo_fila + modulo_columna);
		double distancia;

		for(auto it = sensores.regalos.begin()+1; it != sensores.regalos.end(); ++it){
			modulo_fila = pow((it->first - origen.fila), 2);
			modulo_columna = pow((it->second - origen.columna), 2);
			distancia = sqrt(modulo_fila + modulo_columna);

			if(distancia < mejor_distancia){
				mejor_distancia = distancia;
				destino.fila = it->first;
				destino.columna = it->second;
			}
		}

		cout << "pathFinding regalo" << endl;
		cout << "origen: " << origen.fila << " " << origen.columna << endl;
		cout << "destino: " << destino.fila << " " << destino.columna << endl;
		num_turnos = 0;
		estoy_ejecutando_plan = pathFinding(origen, destino, plan);
	}

	else if((bien_situado && tiene_regalo && !estoy_ejecutando_plan && rey_localizado && num_turnos > 200) || (bien_situado && tiene_regalo && !estoy_ejecutando_plan && rey_localizado && replanificar)){
		estado origen;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		estado destino = reyes.front();

		double modulo_fila = pow((destino.fila - origen.fila), 2);
		double modulo_columna = pow((destino.columna - origen.columna), 2);
		double mejor_distancia = sqrt(modulo_fila + modulo_columna);
		double distancia;

		for(auto it = reyes.begin(); it != reyes.end(); ++it){
			modulo_fila = pow((it->fila - origen.fila), 2);
			modulo_columna = pow((it->columna - origen.columna), 2);
			distancia = sqrt(modulo_fila + modulo_columna);

			if(distancia < mejor_distancia){
				mejor_distancia = distancia;
				destino.fila = it->fila;
				destino.columna = it->columna;
			}
		}

		cout << "pathFinding reyes" << endl;
		cout << "origen: " << origen.fila << " " << origen.columna << endl;
		cout << "destino: " << destino.fila << " " << destino.columna << endl;
		num_turnos = 0;
		replanificar = false;
		estoy_ejecutando_plan = pathFinding(origen, destino, plan);
	}
	
	// Decidir acción
	Action accion;

	if(sensores.superficie[2] == 'r' && tiene_regalo){
		if(sensores.objetoActivo == '4'){
			accion = actGIVE;
			tiene_regalo = false;
		}

		else{
			if(sensores.terreno[0] == 'A' || sensores.terreno[0] == 'B')
				accion = actTURN_L;
			
			else{
				if(ultimaAccion == actPUSH)
					accion = actPOP;

				else
					accion = actPUSH;
			}
		}
	}

	// Si va a por un objeto
	else if(va_a_por_objeto){
		accion = proxima_accion;
		va_a_por_objeto = false;
	}

	else if(sensores.superficie[2] == '4'){
		if(tiene_regalo)
			accion = actTURN_R;
						
		else{
			accion = actPUSH;
			tiene_regalo = true;
			num_objetos++;
			va_a_por_objeto = true;
			proxima_accion = actPICKUP;
		}
	}

	else if (estoy_ejecutando_plan && !error_plan){
		PintaPlan(plan);
		accion = plan.front();
		plan.erase(plan.begin());
		if (plan.size()==0){
			estoy_ejecutando_plan = false;
		}
	}

	// Si va a por un PK
	else if(va_a_por_PK){
		accion = proxima_accion;
		va_a_por_PK = false;
	}

	else if(va_a_por_rey){
		accion = proxima_accion;
		va_a_por_rey = false;
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

	else if(HayReyCercaDerecha(sensores.superficie) && tiene_regalo){
		va_a_por_rey = true;
		accion = actFORWARD;
		proxima_accion = actTURN_R;
	}

	else if(HayReyCercaIzquierda(sensores.superficie) && tiene_regalo){
		va_a_por_rey = true;
		accion = actFORWARD;
		proxima_accion = actTURN_L;
	}

	else if(HayPKCercaRecto(sensores.superficie) && tiene_regalo){
		accion = actFORWARD;
	}

	// Si hay algo en la superficie muy cerca
	else if(sensores.superficie[1] != '_' || sensores.superficie[2] != '_' || sensores.superficie[3] != '_'){

		// Si hay un objeto delante
		if(sensores.superficie[2] != '_' && sensores.superficie[2] != 'a' && sensores.superficie[2] != 'l' && sensores.superficie[2] != 'd' && sensores.superficie[2] != 'r'){

			// Si está en un bosque, en agua o tiene la mochila llena, esquiva el objeto
			if(sensores.terreno[0] == 'B' || sensores.terreno[0] == 'A')
				accion = actTURN_L;


			// Si no tiene ningún objeto, lo coge
			else if(num_objetos == 0){
				if(sensores.superficie[2] != '0'){
					accion = actPICKUP;
					num_objetos++;
					if(sensores.superficie[2] == '1')
						tiene_bikini = true;

					else if(sensores.superficie[2] == '2')
						tiene_botas = true;

					else if(sensores.superficie[2] == '3')
						tiene_llave = true;

					else if(sensores.superficie[2] == '4')
						tiene_regalo = true;
				}

				else
					accion = actTURN_R;
			}

			// Si tiene algún objeto, coge el objeto del suelo si aún no lo tiene en la mochila
			else{
				switch(sensores.superficie[2]){
					case '0':
						accion = actTURN_L;
						break;

					case '1':
						if(tiene_bikini)
							accion = actTURN_L;
						
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
							accion = actTURN_L;
						
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
							accion = actTURN_L;

						else{
							accion = actPUSH;
							tiene_llave = true;
							num_objetos++;
							va_a_por_objeto = true;
							proxima_accion = actPICKUP;
						}
						break;

					case '4':
						if(tiene_regalo)
							accion = actTURN_L;
						
						else{
							accion = actPUSH;
							tiene_regalo = true;
							num_objetos++;
							va_a_por_objeto = true;
							proxima_accion = actPICKUP;
						}
						break;
				}
			}
		}

		// Si hay un objeto en el segundo elemento del vector superficie, ir hacia delante e indicar que va a por el objeto
		else if(sensores.superficie[1] != '_' && sensores.superficie[1] != 'a' && sensores.superficie[1] != 'l' && sensores.superficie[1] != 'd' && sensores.superficie[1] != 'r'){
			va_a_por_objeto = true;
			proxima_accion = actTURN_L;
			accion = actFORWARD;
		}

		// Si hay un objeto en el cuarto elemento del vector superficie, ir hacia delante e indicar que va a por el objeto
		else if(sensores.superficie[3] != '_' && sensores.superficie[3] != 'a' && sensores.superficie[3] != 'l' && sensores.superficie[3] != 'd' && sensores.superficie[3] != 'r'){
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

		else if(sensores.superficie[2] == 'r')
			accion = actTURN_L;

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
	num_turnos++;
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

 void ComportamientoJugador::ActualizarLocalizacionReyNorte(const vector< unsigned char> superficie, estado & rey){
 	if(superficie[1] == 'r'){
 		rey.fila = fil;
 		rey.columna = col-1;
 		rey.orientacion = '0';
 	}

 	else if(superficie[2] == 'r'){
 		rey.fila = fil;
 		rey.columna = col;
 		rey.orientacion = '0';
 	}

 	else if(superficie[3] == 'r'){
  		rey.fila = fil;
 		rey.columna = col+1;
 		rey.orientacion = '0';		
 	}

 	else if(superficie[4] == 'r'){
  		rey.fila = fil-1;
 		rey.columna = col-2;
 		rey.orientacion = '0';		
 	}

 	else if(superficie[5] == 'r'){
  		rey.fila = fil-1;
 		rey.columna = col-1;
 		rey.orientacion = '0';		
 	}

 	else if(superficie[6] == 'r'){
  		rey.fila = fil-1;
 		rey.columna = col;
 		rey.orientacion = '0';		
 	}

 	else if(superficie[7] == 'r'){
  		rey.fila = fil-1;
 		rey.columna = col+1;
 		rey.orientacion = '0';		
 	}

 	else if(superficie[8] == 'r'){
  		rey.fila = fil-1;
 		rey.columna = col+2;
 		rey.orientacion = '0';		
 	}

 	else if(superficie[9] == 'r'){
  		rey.fila = fil-2;
 		rey.columna = col-3;
 		rey.orientacion = '0';		
 	}

 	else if(superficie[10] == 'r'){
 		rey.fila = fil-2;
 		rey.columna = col-2;
 		rey.orientacion = '0'; 		
 	}

 	else if(superficie[11] == 'r'){
  		rey.fila = fil-2;
 		rey.columna = col-1;
 		rey.orientacion = '0';		
 	}

 	else if(superficie[12] == 'r'){
 		rey.fila = fil-2;
 		rey.columna = col;
 		rey.orientacion = '0'; 		
 	}

 	else if(superficie[13] == 'r'){
  		rey.fila = fil-2;
 		rey.columna = col+1;
 		rey.orientacion = '0';		
 	}

 	else if(superficie[14] == 'r'){
  		rey.fila = fil-2;
 		rey.columna = col+2;
 		rey.orientacion = '0';		
 	}

 	else if(superficie[15] == 'r'){
  		rey.fila = fil-2;
 		rey.columna = col+3;
 		rey.orientacion = '0';		
 	}

 }

 void ComportamientoJugador::ActualizarLocalizacionReyEste(const vector< unsigned char> superficie, estado & rey){
 	if(superficie[1] == 'r'){
 		rey.fila = fil-1;
 		rey.columna = col;
 		rey.orientacion = '1';
 	}

 	else if(superficie[2] == 'r'){
 		rey.fila = fil;
 		rey.columna = col;
 		rey.orientacion = '1';
 	}

 	else if(superficie[3] == 'r'){
  		rey.fila = fil+1;
 		rey.columna = col;
 		rey.orientacion = '1';		
 	}

 	else if(superficie[4] == 'r'){
  		rey.fila = fil-2;
 		rey.columna = col+1;
 		rey.orientacion = '1';		
 	}

 	else if(superficie[5] == 'r'){
  		rey.fila = fil-1;
 		rey.columna = col+1;
 		rey.orientacion = '1';		
 	}

 	else if(superficie[6] == 'r'){
  		rey.fila = fil;
 		rey.columna = col+1;
 		rey.orientacion = '1';		
 	}

 	else if(superficie[7] == 'r'){
  		rey.fila = fil+1;
 		rey.columna = col+1;
 		rey.orientacion = '1';		
 	}

 	else if(superficie[8] == 'r'){
  		rey.fila = fil+2;
 		rey.columna = col+1;
 		rey.orientacion = '1';		
 	}

 	else if(superficie[9] == 'r'){
  		rey.fila = fil-3;
 		rey.columna = col+2;
 		rey.orientacion = '1';		
 	}

 	else if(superficie[10] == 'r'){
 		rey.fila = fil-2;
 		rey.columna = col+2;
 		rey.orientacion = '1'; 		
 	}

 	else if(superficie[11] == 'r'){
  		rey.fila = fil-1;
 		rey.columna = col+2;
 		rey.orientacion = '1';		
 	}

 	else if(superficie[12] == 'r'){
  		rey.fila = fil;
 		rey.columna = col+2;
 		rey.orientacion = '1';		
 	}

 	else if(superficie[13] == 'r'){
  		rey.fila = fil+1;
 		rey.columna = col+2;
 		rey.orientacion = '1';		
 	}

 	else if(superficie[14] == 'r'){
  		rey.fila = fil+2;
 		rey.columna = col+2;
 		rey.orientacion = '1';		
 	}

 	else if(superficie[15] == 'r'){
  		rey.fila = fil+2;
 		rey.columna = col+2;
 		rey.orientacion = '1';		
 	}
 }

 void ComportamientoJugador::ActualizarLocalizacionReySur(const vector< unsigned char> superficie, estado & rey){
 	if(superficie[1] == 'r'){
 		rey.fila = fil;
 		rey.columna = col+1;
 		rey.orientacion = '2';
 	}

 	else if(superficie[2] == 'r'){
 		rey.fila = fil;
 		rey.columna = col;
 		rey.orientacion = '2';
 	}

 	else if(superficie[3] == 'r'){
  		rey.fila = fil;
 		rey.columna = col-1;
 		rey.orientacion = '2';		
 	}

 	else if(superficie[4] == 'r'){
  		rey.fila = fil+1;
 		rey.columna = col+2;
 		rey.orientacion = '2';		
 	}

 	else if(superficie[5] == 'r'){
  		rey.fila = fil+1;
 		rey.columna = col+1;
 		rey.orientacion = '2';		
 	}

 	else if(superficie[6] == 'r'){
  		rey.fila = fil+1;
 		rey.columna = col;
 		rey.orientacion = '2';		
 	}

 	else if(superficie[7] == 'r'){
  		rey.fila = fil+1;
 		rey.columna = col-1;
 		rey.orientacion = '2';		
 	}

 	else if(superficie[8] == 'r'){
  		rey.fila = fil+1;
 		rey.columna = col-2;
 		rey.orientacion = '2';		
 	}

 	else if(superficie[9] == 'r'){
  		rey.fila = fil+2;
 		rey.columna = col+3;
 		rey.orientacion = '2';		
 	}

 	else if(superficie[10] == 'r'){
 		rey.fila = fil+2;
 		rey.columna = col+2;
 		rey.orientacion = '2'; 		
 	}

 	else if(superficie[11] == 'r'){
  		rey.fila = fil+2;
 		rey.columna = col+1;
 		rey.orientacion = '2';		
 	}

 	else if(superficie[12] == 'r'){
  		rey.fila = fil+2;
 		rey.columna = col;
 		rey.orientacion = '2';		
 	}

 	else if(superficie[13] == 'r'){
  		rey.fila = fil+2;
 		rey.columna = col-1;
 		rey.orientacion = '2';		
 	}

 	else if(superficie[14] == 'r'){
  		rey.fila = fil+2;
 		rey.columna = col-2;
 		rey.orientacion = '2';		
 	}

 	else if(superficie[15] == 'r'){
  		rey.fila = fil+2;
 		rey.columna = col-3;
 		rey.orientacion = '2';		
 	}
 }

 void ComportamientoJugador::ActualizarLocalizacionReyOeste(const vector< unsigned char> superficie, estado & rey){
 	if(superficie[1] == 'r'){
 		rey.fila = fil+1;
 		rey.columna = col;
 		rey.orientacion = '3';
 	}

 	else if(superficie[2] == 'r'){
 		rey.fila = fil;
 		rey.columna = col;
 		rey.orientacion = '3';
 	}

 	else if(superficie[3] == 'r'){
  		rey.fila = fil-1;
 		rey.columna = col;
 		rey.orientacion = '3';		
 	}

 	else if(superficie[4] == 'r'){
  		rey.fila = fil+2;
 		rey.columna = col-1;
 		rey.orientacion = '3';		
 	}

 	else if(superficie[5] == 'r'){
  		rey.fila = fil+1;
 		rey.columna = col-1;
 		rey.orientacion = '3';		
 	}

 	else if(superficie[6] == 'r'){
  		rey.fila = fil;
 		rey.columna = col-1;
 		rey.orientacion = '3';		
 	}

 	else if(superficie[7] == 'r'){
  		rey.fila = fil-1;
 		rey.columna = col-1;
 		rey.orientacion = '3';		
 	}

 	else if(superficie[8] == 'r'){
  		rey.fila = fil-2;
 		rey.columna = col-1;
 		rey.orientacion = '3';		
 	}

 	else if(superficie[9] == 'r'){
  		rey.fila = fil+3;
 		rey.columna = col-2;
 		rey.orientacion = '3';		
 	}

 	else if(superficie[10] == 'r'){
 		rey.fila = fil+2;
 		rey.columna = col-2;
 		rey.orientacion = '3'; 		
 	}

 	else if(superficie[11] == 'r'){
  		rey.fila = fil+1;
 		rey.columna = col-2;
 		rey.orientacion = '3';		
 	}

 	else if(superficie[12] == 'r'){
  		rey.fila = fil;
 		rey.columna = col-2;
 		rey.orientacion = '3';		
 	}

 	else if(superficie[13] == 'r'){
  		rey.fila = fil-1;
 		rey.columna = col-2;
 		rey.orientacion = '3';		
 	}

 	else if(superficie[14] == 'r'){
  		rey.fila = fil-2;
 		rey.columna = col-2;
 		rey.orientacion = '3';		
 	}

 	else if(superficie[15] == 'r'){
  		rey.fila = fil-3;
 		rey.columna = col-2;
 		rey.orientacion = '3';		
 	}
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

bool ComportamientoJugador::HayReyCercaDerecha(const vector< unsigned char> superficie){
	if(superficie[3] == 'r' || superficie[7] == 'r' || superficie[8] == 'r' || superficie[13] == 'r' || superficie[14] == 'r' || superficie[15] == 'r')
		return true;

	return false;
}

bool ComportamientoJugador::HayReyCercaIzquierda(const vector< unsigned char> superficie){
	if(superficie[1] == 'r' || superficie[4] == 'r' || superficie[5] == 'r' || superficie[9] == 'r' || superficie[10] == 'r' || superficie[11] == 'r')
		return true;

	return false;
}

bool ComportamientoJugador::HayReyCercaRecto(const vector< unsigned char> superficie){
	if(superficie[6] == 'r' || superficie[12] == 'r')
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

	if(!tiene_regalo)
		if(superficie[3] == '4' || superficie[7] == '4' || superficie[8] == '4' || superficie[13] == '4' || superficie[14] == '4' || superficie[15] == '4')
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

	if(!tiene_regalo)
		if(superficie[1] == '4' || superficie[4] == '4' || superficie[5] == '4' || superficie[9] == '4' || superficie[10] == '4' || superficie[1] == '4')
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

	if(!tiene_regalo)
		if(superficie[6] == '4' || superficie[12] == '4')
			return true;

	return false;
}

bool ComportamientoJugador::HayReyCerca(const vector< unsigned char> superficie){
	return (superficie[1] == 'r' || superficie[2] == 'r' | superficie[3] == 'r' || superficie[4] == 'r' || superficie[5] == 'r' || superficie[6] == 'r' || superficie[7] == 'r' || superficie[8] == 'r'
		|| superficie[9] == 'r' || superficie[10] == 'r' || superficie[11] == 'r' || superficie[12] == 'r' || superficie[13] == 'r' || superficie[14] == 'r' || superficie[15] == 'r');
}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan){
	//Borro la lista
	plan.clear();

	estado estado_actual = origen;

	list<nodo> abiertos, cerrados;

	// Meto el nodo inicial en abiertos
	nodo inicial;
	list<Action> nodos;

	inicial.state = origen;

	abiertos.push_back(inicial);

	int difF = abs(origen.fila - destino.fila);
	int difC = abs(origen.columna - destino.columna);

	nodo nodo_actual;
	list<Action> acciones_nodo_actual;

	while(!abiertos.empty() && nodo_actual.state != destino){

		nodo_actual = abiertos.front();
		acciones_nodo_actual = nodo_actual.acciones;

		abiertos.pop_front();

		nodo hijo;

		int i;

		for(i = 0; i < 3; ++i){

			switch(nodo_actual.state.orientacion){
			
				case 0:
					if(i == 0){
						hijo.state.fila = nodo_actual.state.fila - 1;
						hijo.state.columna = nodo_actual.state.columna;
						hijo.state.orientacion = 0;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actFORWARD);

					}

					else if(i == 1){
						hijo.state.fila = nodo_actual.state.fila;
						hijo.state.columna = nodo_actual.state.columna;
						hijo.state.orientacion = 1;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actTURN_R);
					}	

					else{
						hijo.state.fila = nodo_actual.state.fila;
						hijo.state.columna = nodo_actual.state.columna;
						hijo.state.orientacion = 3;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actTURN_L);
					}

					break;

				case 1:
					if(i == 0){
						hijo.state.fila = nodo_actual.state.fila;
						hijo.state.columna = nodo_actual.state.columna + 1;
						hijo.state.orientacion = 1;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actFORWARD);
					}

					else if(i == 1){
						hijo.state.fila = nodo_actual.state.fila;
						hijo.state.columna = nodo_actual.state.columna;
						hijo.state.orientacion = 2;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actTURN_R);
					}

					else{
						hijo.state.fila = nodo_actual.state.fila;
						hijo.state.columna = nodo_actual.state.columna;
						hijo.state.orientacion = 0;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actTURN_L);
					}

					break;

				case 2:
					if(i == 0){
						hijo.state.fila = nodo_actual.state.fila + 1;
						hijo.state.columna = nodo_actual.state.columna;
						hijo.state.orientacion = 2;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actFORWARD);
					}

					else if(i == 1){
						hijo.state.fila = nodo_actual.state.fila;
						hijo.state.columna = nodo_actual.state.columna;
						hijo.state.orientacion = 3;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actTURN_R);
					}

					else{
						hijo.state.fila = nodo_actual.state.fila;
						hijo.state.columna = nodo_actual.state.columna;
						hijo.state.orientacion = 1;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actTURN_L);
					}

					break;

				case 3:
					if(i == 0){
						hijo.state.fila = nodo_actual.state.fila;
						hijo.state.columna = nodo_actual.state.columna - 1;
						hijo.state.orientacion = 3;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actFORWARD);
					}

					else if(i == 1){
						hijo.state.fila = nodo_actual.state.fila;
						hijo.state.columna = nodo_actual.state.columna;
						hijo.state.orientacion = 0;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actTURN_R);
					}

					else{
						hijo.state.fila = nodo_actual.state.fila;
						hijo.state.columna = nodo_actual.state.columna;
						hijo.state.orientacion = 2;
						hijo.acciones = acciones_nodo_actual;
						hijo.acciones.push_back(actTURN_L);
					}

					break;												

			}

			if(!find_nodo(abiertos, hijo) && !find_nodo(cerrados, hijo) && hijo.state.fila >= 3 && hijo.state.fila <= 96 && hijo.state.columna >= 3 && hijo.state.columna <= 96
				&& abs(hijo.state.fila - destino.fila) <= (difF+20) && abs(hijo.state.columna - destino.columna) <= (difC+20)){
				if(mi_mapa_auxiliar[hijo.state.fila][hijo.state.columna] == 'S' || mi_mapa_auxiliar[hijo.state.fila][hijo.state.columna] == 'T' || mi_mapa_auxiliar[hijo.state.fila][hijo.state.columna] == '?'
					|| mi_mapa_auxiliar[hijo.state.fila][hijo.state.columna] == 'B' || mi_mapa_auxiliar[hijo.state.fila][hijo.state.columna] == 'A')
					abiertos.push_back(hijo);
			}
		}

		cerrados.push_back(nodo_actual);
	}

	if(abiertos.empty())
		return false;
	else{
		for(auto it = nodo_actual.acciones.begin(); it != nodo_actual.acciones.end(); ++it)
			cout << *it << " ";
		cout << endl;
		plan = nodo_actual.acciones;
		return true;
	}
}

bool ComportamientoJugador::find_nodo(const list<nodo> & l, const nodo & n){
	for(auto it = l.begin(); it != l.end(); ++it){
		if(*it == n)
			return true;
	}

	return false;
}

bool ComportamientoJugador::find_estado(const list<estado> & l, const estado & r){
	for(auto it = l.begin(); it != l.end(); ++it){
		if(*it == r)
			return true;
	}

	return false;
}

void ComportamientoJugador::PintaPlan(list<Action> plan){
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
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

   estoy_ejecutando_plan = false;
   tiene_regalo = false;
   error_plan = false;
   plan.clear();
   num_turnos = 0;
   va_a_por_rey = false;
   replanificar = false;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
