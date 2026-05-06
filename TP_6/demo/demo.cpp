/**
 * @file demo.cpp
 * @brief Simulation d'impact : un disque de Lennard-Jones tombe sur une membrane.
 *
 * Disque circulaire de ~1600 particules avec vitesse initiale (0, -10) tombant
 * sur une membrane horizontale de particules immobiles. Le système est intégré
 * en temps par le schéma de Störmer–Verlet, et les positions/vitesses sont
 * exportées au format VTK pour visualisation (Paraview).
 *
 * @note Depuis la refactorisation d'@c Univers, la méthode @c all_forces ne
 *       calcule plus que les forces inter-particules. La gravité et la
 *       réflexion aux bords doivent désormais être appelées explicitement.
 */

#include "Particule.hxx"
#include "Univers.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>
#include <stdexcept>
#include <filesystem>   // C++17, pour créer le dossier frames/

/**
 * @brief Collecte des pointeurs vers toutes les particules de l'univers.
 *
 * Pratique pour parcourir toutes les particules sans connaître la structure
 * en cellules (sortie VTK, statistiques globales, etc.).
 *
 * @param univers Univers source.
 * @return Vecteur de pointeurs vers chaque particule.
 */
std::vector<Particule*> collecterToutesLesParticules(Univers& univers) {
    std::vector<Particule*> liste_pointeurs;
    for (Cellule& cell : univers.getCellules())
        for (Particule& p : cell)
            liste_pointeurs.push_back(&p);
    return liste_pointeurs;
}

/**
 * @brief Sauvegarde l'état de l'univers au format VTU (Paraview).
 *
 * Écrit un fichier UnstructuredGrid avec les positions, vitesses et masses
 * de chaque particule. La 3e composante (z) est toujours à 0 (simulation 2D).
 *
 * @param nom  Chemin du fichier de sortie (ex: "frames/frame_42.vtu").
 * @param univ Univers à sauvegarder.
 * @throws std::runtime_error Si le fichier ne peut pas être ouvert.
 */
void sauvegarderVTK(const std::string& nom, Univers& univ) {
    std::ofstream out(nom);
    if (!out.is_open())
        throw std::runtime_error("Erreur : impossible d'ouvrir " + nom);

    std::vector<Particule*> ptrs = collecterToutesLesParticules(univ);
    int nbPart = ptrs.size();

    out << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n"
        << "  <UnstructuredGrid>\n"
        << "    <Piece NumberOfPoints=\"" << nbPart << "\" NumberOfCells=\"0\">\n";

    out << "      <Points>\n"
        << "        <DataArray Name=\"Points\" type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    for (Particule* p : ptrs)
        out << p->getPosition(0) << " " << p->getPosition(1) << " 0 ";
    out << "\n        </DataArray>\n      </Points>\n";

    out << "      <PointData Vectors=\"Velocity\">\n"
        << "        <DataArray Name=\"Velocity\" type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    for (Particule* p : ptrs)
        out << p->getVitesse(0) << " " << p->getVitesse(1) << " 0 ";
    out << "\n        </DataArray>\n";

    out << "        <DataArray Name=\"Masse\" type=\"Float32\" format=\"ascii\">\n";
    for (Particule* p : ptrs)
        out << p->getMas() << " ";
    out << "\n        </DataArray>\n      </PointData>\n";

    out << "      <Cells>\n"
        << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"></DataArray>\n"
        << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"></DataArray>\n"
        << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\"></DataArray>\n"
        << "      </Cells>\n";

    out << "    </Piece>\n  </UnstructuredGrid>\n</VTKFile>\n";
}

/**
 * @brief Intégration en temps par le schéma de Störmer–Verlet.
 *
 * Schéma symplectique d'ordre 2 :
 *   x(t+dt) = x(t) + dt * v(t) + 0.5 * dt^2 * F(t)/m
 *   v(t+dt) = v(t) + 0.5 * dt * (F(t) + F(t+dt)) / m
 *
 * À chaque pas, les forces sont recalculées via @c all_forces, complétées
 * par la gravité et les conditions aux bords, puis la grille est mise à
 * jour via @c maj_cellules. Toutes les 5 itérations, on exporte un fichier
 * VTK pour visualisation. Toutes les 1000 itérations, l'énergie cinétique
 * est ramenée à @p Ec_cible (thermostat).
 *
 * @param univers   Univers à intégrer (modifié).
 * @param nb_part   Nombre de particules (taille de Fold).
 * @param dim       Dimension de l'espace.
 * @param tend      Temps final.
 * @param dt        Pas de temps.
 * @param Ec_cible  Énergie cinétique cible pour le thermostat.
 */
void Stormer_Verlet(Univers& univers, int nb_part, int dim,
                    double tend, double dt, double Ec_cible) {

    std::vector<double> Fold(nb_part * dim, 0.0);

    /* AVANT (all_forces calculait LJ + gravité + réflexion en interne) :
       univers.all_forces();                                                 */
    // APRÈS : on appelle chaque étape explicitement.
    univers.all_forces();
    univers.applique_Potentiel_gravitationnel();
    univers.appliquer_reflection();

    double t = 0.0;
    int    step = 0;

    sauvegarderVTK("frames/frame_0.vtu", univers);

    while (t < tend) {
        t += dt;
        ++step;

        // Mise à jour des positions (premier demi-pas Verlet) + sauvegarde des forces
        for (Cellule& cell : univers.getCellules()) {
            for (Particule& p : cell) {
                int    i  = p.getId();
                double mi = p.getMas();
                for (int k = 0; k < dim; ++k) {
                    double xi = p.getPosition(k);
                    double vi = p.getVitesse(k);
                    double Fi = p.getForce(k);
                    p.setPosition(k, xi + dt * (vi + 0.5 / mi * Fi * dt));
                    Fold[i * dim + k] = Fi;
                }
            }
        }

        univers.maj_cellules();

        /* AVANT (all_forces faisait gravité + réflexion implicitement) :
           univers.all_forces();                                              */
        // APRÈS : étapes explicites pour conserver le même comportement.
        univers.all_forces();
        univers.applique_Potentiel_gravitationnel();
        univers.appliquer_reflection();

        // Mise à jour des vitesses (second demi-pas Verlet)
        for (Cellule& cell : univers.getCellules()) {
            for (Particule& p : cell) {
                int    i  = p.getId();
                double mi = p.getMas();
                for (int k = 0; k < dim; ++k) {
                    double vi     = p.getVitesse(k);
                    double Fi     = p.getForce(k);
                    double Fold_i = Fold[i * dim + k];
                    p.setVitesse(k, vi + dt * 0.5 / mi * (Fi + Fold_i));
                }
            }
        }

        // Sauvegarde + check NaN
        if (step % 5 == 0) {
            std::cout << "Progression t = " << t << " / " << tend
                      << "\r" << std::flush;
            for (Cellule& cell : univers.getCellules()) {
                for (Particule& p : cell) {
                    if (std::isnan(p.getVitesse(0)) || std::isnan(p.getPosition(0))) {
                        std::cout << "\nNaN détecté à step=" << step
                                  << " id=" << p.getId() << "\n";
                        return;
                    }
                }
            }
            sauvegarderVTK("frames/frame_" + std::to_string(step) + ".vtu",
                           univers);
        }

        // Thermostat brutal : rescaling de l'énergie cinétique
        if (step % 1000 == 0)
            univers.rescaleV(Ec_cible);
    }

    sauvegarderVTK("etat_final.vtu", univers);
    std::cout << "\nSimulation terminée à t = " << t << "\n";
}

int main() {
    try {
        // === Paramètres de la simulation ===
        const int    dim   = 2;
        const double Lx    = 250.0;   ///< Largeur du domaine.
        const double Ly    = 180.0;   ///< Hauteur du domaine.
        const double rcut  = 2.5;     ///< Rayon de coupure LJ.
        const double dt    = 0.005;   ///< Pas de temps.
        const double tend  = 29.5;    ///< Temps final de simulation.
        const double sigma = 1.0;
        const double d0    = std::pow(2.0, 1.0/6.0) * sigma;  ///< Distance d'équilibre LJ.

        // S'assure que le dossier frames/ existe (C++17)
        std::filesystem::create_directory("frames");

        const int nb_part_max = 40 * 40 + 160 * 40;
        Univers univers(dim, nb_part_max, Lx, Ly, rcut);

        int id = 0;

        // === 1. Disque (projectile) ===
        const double R  = 15.0 * d0;
        const double cx = Lx / 2.0;
        const double cy = 120.0;
        for (int j = 0; j < 40; ++j) {
            for (int i = 0; i < 40; ++i) {
                double x  = cx + i*d0 - 20*d0;
                double y  = cy + j*d0 - 20*d0;
                double dx = x - cx;
                double dy = y - cy;
                if (dx*dx + dy*dy <= R*R) {
                    Particule p({x, y}, {0.0, -10.0}, {0.0, 0.0},
                                1.0, id++, Categorie::Proton);
                    univers.ajouterParticule(p);
                }
            }
        }

        // === 2. Membrane horizontale (cible) ===
        const int n = static_cast<int>(Lx / d0);
        for (int j = 1; j < 40; ++j) {
            for (int i = 0; i < n; ++i) {
                Particule p({i*d0, j*d0}, {0.0, 0.0}, {0.0, 0.0},
                            1.001, id++, Categorie::Proton);
                univers.ajouterParticule(p);
            }
        }

        std::cout << "Particules placées : " << id << "\n";

        const double Ec_D    = 0.005 * id;   ///< Énergie cinétique cible.
        const int    nb_part = id;

        // === Lancement de la simulation ===
        auto start = std::chrono::steady_clock::now();
        Stormer_Verlet(univers, nb_part, dim, tend, dt, Ec_D);
        auto end_t = std::chrono::steady_clock::now();

        std::cout << "Temps écoulé : "
                  << std::chrono::duration<double>(end_t - start).count()
                  << " s\n";
    }
    catch (const std::exception& e) {
        std::cerr << "\n[ ARRÊT DE LA SIMULATION ]\n"
                  << "Raison : " << e.what() << "\n";
        return 1;
    }

    return 0;
}