#include "GestionnaireInstructions.h"

GestionnaireInstructions::GestionnaireInstructions() {
	debut_ = false;
    finProg_ = false;
    uint8_t temp = 0;
    mem_.lecture(0, &temp);
    tailleProg_ = 0x0000 | (temp << 8);
    mem_.lecture(1, &temp);
    tailleProg_ |= temp;
}

void GestionnaireInstructions::executer() {
    uint8_t adresseBoucle = 0, iterationsRestantes = 0;
	Instruction uneInstruction;
    uint8_t donneesLues[2];                        //Le premier 8 octets est l'opcode, le deuxieme 8 octets est l'operande.    
	uint8_t vitesseAvant = 0;
    uint8_t vitesseArriere = 0;
    for(uint16_t i = 2; i < tailleProg_ && !finProg_; i+=2){
		mem_.lecture(i, donneesLues, 2);
		uneInstruction.setOpcode(donneesLues[0]);
        uneInstruction.setOperande(donneesLues[1]);
        if(debut_) {
            switch(uneInstruction.getOpcode()){
                    case att: 
                        delai_ms(uneInstruction.getOperande() * 25);       //25 fois la valeur de l'operande
                        break;
                    case dal:
                        del_.allumerDel(uneInstruction.getOperande());
                        break;
                    case det:
                        del_.fermerDel(uneInstruction.getOperande()); 
                        break;
                    case sgo: 
                        piezo_.debutSon();
                        piezo_.setFrequence(uneInstruction.getOperande());
                        break;
                    case sar: 
                        piezo_.arretSon();
                        break;
                    case mar: 
                        moteur_.arreter();
                        break;
                    case 0x61:
                        moteur_.arreter();
                        break;
                    case mav:
                        vitesseAvant = uneInstruction.getOperande();
                        moteur_.avancer(vitesseAvant, vitesseAvant); 
                        break;
                    case mre: 
                        vitesseArriere = uneInstruction.getOperande();
                        moteur_.reculer(vitesseArriere, vitesseArriere);
                        break;
                    case trd: 
                        moteur_.tournerDroite();
                        break;
                    case trg: 
                        moteur_.tournerGauche();
                        break;
                    case dbc:
                        adresseBoucle = i; //afin de ne jamais refaire cette operation
                        iterationsRestantes = uneInstruction.getOperande();
                        break;
                    case fbc:
                    	if(iterationsRestantes > 0) {
                    		i = adresseBoucle;
                            iterationsRestantes--;
                    	}
                        break;
                    case fin: 
                        finProg_ = true;
                        moteur_.arreter();
                        break;
            }
        }
        else if(uneInstruction.getOpcode() == dbt) {
            debut_ = true;
        }
	}
}
