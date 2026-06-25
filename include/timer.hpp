#pragma once

#include <chrono>

// Estensione al progetto, tenuta separata dal codice gia' esistente.
// Classe Timer per la misurazione dei tempi di esecuzione, con interfaccia
// tic()/toc() nello stile delle esercitazioni:
//   - tic()  avvia (o riavvia) il cronometro
//   - toc()  restituisce il tempo trascorso dall'ultimo tic() in millisecondi

class Timer
{
private:
    std::chrono::high_resolution_clock::time_point inizio_;

public:
    Timer() { tic(); }

    void tic() {
        inizio_ = std::chrono::high_resolution_clock::now();
    }

    double toc() const {
        auto fine = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(fine - inizio_).count();
    }
};