#ifndef FEAR_H
#define FEAR_H
#include <QVector>
#include <QString>

class MFear
{
public:
    explicit MFear()
    {

    }

    QString getFear(int month, int day) const
    {
        if (month > 0 && month <13 && day > 0 )
        return fears[month-1][day-1];

       return "";
    }

private:
   const QVector<QStringList> fears =
    {
        {
            "JOUR del'AN",
            "Basile",
            "Geneviève",
            "Odilon",
            "Edouard",
            "Epiphanie",
            "Raymond",
            "Lucien",
            "Alix",
            "Guillaume",
            "Paulin",
            "Tatiana",
            "Yvette",
            "Nina",
            "Rémi",
            "Marcel",
            "Roseline",
            "Prisca",
            "Marius",
            "Sébastien",
            "Agnès",
            "Vincent",
            "Barnard",
            "Fr. de Sales",
            "Conv. S. Paul",
            "Paul",
            "Angèle",
            "Th. d'Aquin",
            "Gildas",
            "Martine",
            "Marcelle",
        },
        {
            "Ella",
            "Présentation",
            "Blaise",
            "Véronique",
            "Agathe",
            "Gaston",
            "Eugénie",
            "Jacqueline",
            "Appoline",
            "Arnaud",
            "N.-D. Lourdes",
            "Félix",
            "Mardi-Gras",
            "Cendres",
            "Claude",
            "Julienne",
            "Alexis",
            "Bernadette",
            "Gabin",
            "Aimée",
            "P. Damien",
            "Isabelle",
            "Lazare",
            "Modeste",
            "Roméo",
            "Nestor",
            "Honorine",
            "Romain",
            ""
        },
        {
            "Aubin",
            "Charles le B.",
            "Guénolé",
            "Casimir",
            "Olive",
            "Colette",
            "Félicité",
            "Jean de Dieu",
            "Françoise",
            "Vivien",
            "Rosine",
            "Justine",
            "Rodrigue",
            "Mathilde",
            "Louise",
            "Bénédicte",
            "Patrice",
            "Cyrille",
            "Joseph",
            "PRINTEMPS",
            "Clémence",
            "Léa",
            "Victorien",
            "Cath. de Suè.",
            "Rameaux",
            "Larissa",
            "Habib",
            "Gontran",
            "Gwladys",
            "",
            "Benjamin"
        },
        {
            "Hugues",
            "Sandrine",
            "Richard",
            "Isidore",
            "Irène",
            "Marcellin",
            "J.-B. de la S.",
            "Julie",
            "Gauthier",
            "Fulbert",
            "Stanislas",
            "Jules",
            "Ida",
            "Maxime",
            "Paterne",
            "Benoît-J.",
            "Anicet",
            "Parfait",
            "Emma",
            "Odette",
            "Anselme",
            "Alexandre",
            "Georges",
            "Fidèle",
            "Marc",
            "Alida",
            "Zita",
            "Jour du Souv.",
            "Cath. de Si.",
            "Robert"
        },

        {
            "F. du TRAVAIL",
            "Boris",
            "Phil., Jacq.",
            "Sylvain",
            "Judith",
            "Prudence",
            "Gisèle",
            "VICTOIRE",
            "Pacôme",
            "Solange",
            "Estelle",
            "Jean.-d'Arc",
            "Rolande",
            "Matthias",
            "Denise",
            "Honoré",
            "Pascal",
            "Eric",
            "Yves",
            "Bernardin",
            "Constantin",
            "Emile",
            "Didier",
            "Donatien",
            "Sophie",
            "Bérenger",
            "F. des Mères",
            "Germain",
            "Aymar",
            "Ferdinand",
            "Visitation"
        },

        {
            "Justin",
            "Blandine",
            "Kévin",
            "Clotilde",
            "Igor",
            "Norbert",
            "Gilbert",
            "Médard",
            "Diane",
            "Landry",
            "Barnabé",
            "Guy",
            "Antoine de P.",
            "Elisée",
            "Germaine",
            "J.-Fr. Régis",
            "F. des Pères",
            "Léonce",
            "Romuald",
            "Silvère",
            "ETE",
            "Alban",
            "Audrey",
            "Jean-Bapt.",
            "Prosper",
            "Anthelme",
            "Fernand",
            "Irénée",
            "Pierre, Paul",
            "Martial"
        },
        {
            "Thierry",
            "Martinien",
            "Thomas",
            "Florent",
            "Antoine",
            "Mariette",
            "Raoul",
            "Thibault",
            "Amandine",
            "Ulrich",
            "Benoît",
            "Olivier",
            "Henri, Joël",
            "",
            "Donald",
            "N-D Mt-Carmel",
            "Charlotte",
            "Frédéric",
            "Arsène",
            "Marina",
            "Victor",
            "Marie-Mad.",
            "Brigitte",
            "Christine",
            "Jacques",
            "Anne, Joach.",
            "Nathalie",
            "Samson",
            "Marthe",
            "Juliette",
            "Ignace de L."
        },
        {
            "Alphonse",
            "Julien-Eym.",
            "Lydie",
            "J.-M. Vianney",
            "Abel",
            "Transfiguration",
            "Gaétan",
            "Dominique",
            "Amour",
            "Laurent",
            "Claire",
            "Clarisse",
            "Hippolyte",
            "Evrard",
            "",
            "Armel",
            "Hyacinthe",
            "Hélène",
            "Jean-Eudes",
            "Bernard",
            "Christophe",
            "Fabrice",
            "Rose de L.",
            "Barthélemy",
            "Louis",
            "Natacha",
            "Monique",
            "Augustin",
            "Sabine",
            "Fiacre",
            "Aristide"
        },
        {
            "Gilles",
            "Ingrid",
            "Grégoire",
            "Rosalie",
            "Raïssa",
            "Bertrand",
            "Reine",
            "Nativité N.-D.",
            "Alain",
            "Inès",
            "Adelphe",
            "Apollinaire",
            "Aimé",
            "La Ste Croix",
            "Roland",
            "Edith",
            "Renaud",
            "Nadège",
            "Emilie",
            "Davy",
            "Matthieu",
            "Maurice",
            "AUTOMNE",
            "Thècle",
            "Hermann",
            "Côme, Dam.",
            "Vinc. de P.",
            "Venceslas",
            "Michel",
            "Jérôme"
        },

        {
            "Thér. de l'E.",
            "Léger",
            "Gérard",
            "Fr. d'Assise",
            "Fleur",
            "Bruno",
            "Serge",
            "Pélagie",
            "Denis",
            "Ghislain",
            "Firmin",
            "Wilfried",
            "Géraud",
            "Juste",
            "Thér. d'Avila",
            "Edwige",
            "Baudoin",
            "Luc",
            "René",
            "Adeline",
            "Céline",
            "Elodie	43",
            "Jean de C.",
            "Florentin",
            "Crépin",
            "Dimitri",
            "Emeline",
            "Simon, Jude",
            "Narcisse",
            "Bienvenue",
            "Quentin"
        },

        {
            "",
            "Défunt",
            "Hubert",
            "Charles",
            "Sylvie",
            "Bertille",
            "Carine",
            "Geoffroy",
            "Théodore",
            "Léon",
            "ARMISTICE",
            "Christian",
            "Brice",
            "Sidoine",
            "Albert",
            "Marguerite",
            "Elisabeth",
            "Aude",
            "Tanguy",
            "Edmond",
            "Prés. Marie",
            "Cécile",
            "Clément",
            "Flora",
            "Christ Roi",
            "Delphine",
            "Séverin",
            "Jacq. de la M.",
            "Saturnin",
            "André"
        },
        {
            "Florence",
            "Avent",
            "Xavier",
            "Barbara",
            "Gérald",
            "Nicolas",
            "Ambroise",
            "Im. Concept.",
            "Pierre Fourier",
            "Romaric",
            "Daniel",
            "Jean. Fr.-Ch.",
            "Lucie",
            "Odile",
            "Ninon",
            "Alice",
            "Gaël",
            "Gatien",
            "Urbain",
            "Abraham",
            "Pierre C.",
            "HIVER",
            "Armand",
            "Adèle",
            "NOËL",
            "Etienne",
            "Jean",
            "Innocents",
            "David",
            "Roger",
            "Sylvestre"
        }
    };
};
#endif
