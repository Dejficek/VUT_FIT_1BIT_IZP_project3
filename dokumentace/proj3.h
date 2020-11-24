/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2018/19
 * a pro dokumentaci Javadoc.
 */
 ///@defgroup cluster_memory_operations Functions for memory operations
 ///@defgroup distance_operations Functions for distance calculations
 ///@defgroup iostream Functions to load and print data


/**
 * struktura objektu, uchovávající id a souřadnice objektu.
 */

struct obj_t {
    int id;     /**<Jedinečný identifikátor objektu*/
    float x;    /**<X souřadnice objektu*/
    float y;    /**<Y souřadnice objektu*/
};

/**
 * Struktura clusteru, uchovávající velikost(počet objektů), capacitu(počet objektu, pro které je alokovaná paměť a která nemusí být využitá)
 * a pointery na objekty, který uchovává.
 */

struct cluster_t {
    int size;           /**<Velikost (AKA počet objektů).*/
    int capacity;       /**<Kapacita (AKA počet objektů, pro které je alokována pamět... Tato paměť nemusí být využitá).*/
    struct obj_t *obj;  /**<Pointery na obsažené objekty.*/
};

/*! Alokuje paměť pro cluster
 *@ingroup cluster_memory_operations
 *@param c ukazatel na místo, kde se má alokovat
 *@param cap kapacita je počet objektů, pro který se má paměť alokovat
 *@pre c musí být inicializováno
 *@pre cap musí být větší, než 0. Pokud bude menší nebo roven nule, nebude alokována žádná paměť
 *@post c ven z funkce se předá adresa na místo v paměti, která byla alokována
 *@post cap pokud byla hodnota úspěšná, hodnota cap se uloží do clusteru na místo capacity
 *@return funkce nevrací žádné hodnoty
 */
void init_cluster(struct cluster_t *c, int cap);

/*! dealukej místo pro cluster
 *@ingroup cluster_memory_operations
 *@param c ukazatel na paměť, která má být uvolněna
 *@pre c by nemělo obsahovat NULL
 *@post c bude uvolněno
 *@return funkce nevrací žádné hodnoty
 */
void clear_cluster(struct cluster_t *c);

/*!
 *Externí proměnná CLUSTER_CHUNK s datovým typek int.
 */
extern const int CLUSTER_CHUNK;

/*! realokuje paměť clusteru na novou kapacitu
 *@ingroup cluster_memory_operations
 *@param c ukazatel na cluster, který se má realokovat
 *@param new_cap nová velikost, na kterou se má cluster realokovat
 *@pre c ukazatel se musí předat. Pokud je nulový, program se ukončí
 *@pre new_cap nová kapacita by měla být menší, než aktualní kapacita
 *@pre new_cap nová kapacita nesmí být menší než 0, jinak se program ukončí
 *@post c clusteru c se změní kapacita na novou kapacitu
 *@post new_cap pokud je realokace úspěšná, hodnota new_cap bude nová alokovaná velikost pro kapacitu
 *@return funkce vrátí ukazatel na alokovanou paměť
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/*! funkce clusteru přialokuje další paměť o velikosti nového objektu a přidá clusteru ukazatel na nový objekt
 *@ingroup cluster_memory_operations
 *@param c ukazatel na cluster, kterému se má zvětšit hodnota a do kterého se má přidat nový objekt
 *@param obj objekt o který se musí zvětšit kapacita clusteru a který se přidá do clusteru
 *@pre c pokud se ukazatel nepředá, program se ukončí
 *@post c do clusteru c se navýší alokovaná paměť o velikost nového objektu
 *@post c do clusteru se uloží ukazatel na objekt na pozici capacity clusteru plus jedna
 *@return funkce nevrací žádné hodnoty
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/*! Přidává objekty jednoho clusteru do druhého clusteru a setřídí je
 *@ingroup cluster_memory_operations
 *@param c1 první cluster, do kterého se mají objekty přidat
 *@param c2 druhý cluster, který obsahuje objekty, které se zkopírují do prvního
 *@pre c1 předaná hodnota se nesmí rovnat "NULL", jinak se program ukončí
 *@pre c2 předaná hodnota se nesmí rovnat "NULL", jinak se program ukončí
 *@post c1 paměť clusteru bude přealokovaná na aktualní paměť zvětšenou o velikost*počet objektů druhého clusteru
 *@post c1 do clusteru budou nakopírovány objekty z druhého clusteru
 *@post c2 cluster zůstane nedotčený
 *@return funkce nevrací žádné hodnoty
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/*! Z pole clusterů vymaže cluster na určité pozici a ostatní clustery, které se indexově nacházejí za vymazaným clusterem se indexově posunou o jednu pozici níže
 *@ingroup cluster_memory_operations
 *@param carr pole clusterů, ze kterého se má odstranit cluster
 *@param narr počet clusterů, které se v poli clusterů nachází
 *@param idx pozice clusteru v poli clusterů, který se má vymazat
 *@pre carr neměla by být nulová hodnota
 *@pre narr hodnota by měla být stejná nebo větší, než pozice mazaného clusteru, jinak se program ukončí
 *@pre idx hodnota by měla být menší než hodnota narr, jinak se program ukončí
 *@post carr pole clusterů bude upraveno... Vymaže se požadovaný prvek a ostatní prvky budou posunuty
 *@post idx prek z pole clusterů se vymaže z pozice idx
 *@return funkce vrátí nový počet clusterů v poli clusterů
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/*! Vypočítá vzdálenost mezi dvěma objekty pomocí eukleidovské vzdálenost(trojúhelník)
*@ingroup distance_operations
*@param o1 objekt č. 1 ze kterého se extrahují souřadnice podle kterých se určí poloha v rovině, ze které se vypočítá vzdálenost mezi tímto a druhým objektem
*@param o2 objekt č. 2 ze kterého se extrahují souřadnice podle kterých se určí poloha v rovině, ze které se vypočítá vzdálenost mezi tímto a prvním objektem
*@pre o1 ukazatel by neměl být nulový, jinak se program ukončí
*@pre o2 ukazatel by neměl být nulový, jinak se program ukončí
*@post o1 objekt zůstane nepozměněný. Vypočítá se z něho vzdálenost mezi tímto a druhým objektem
*@post o2 objekt zůstane nepozměněný. Vypočítá se z něho vzdálenost mezi tímto a prvním objektem
*@return funkce navrácí vzdálenost obou objektů v rovině v datovém typu "float"
*/
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/*! Vypočítá vzdálenost mezi dvěma clustery pomocí vzdáleností jednotlivých objektů v obou clusterech pomocí eukleidovské vdálenosti(trojúhelník)
*@ingroup distance_operations
*@param c1 první cluster, který obsahuje objekty ze kterých se počítá vzdálenost s objekty obsaženými v druhém clusteru. Porovnává se každý objekt s každým a nejmenší hodnota je výsledná hodnota
*@param c2 druhý cluster, který obsahuje objekty ze kterých se počítá vzdálenost s objekty obsaženými v prvním clusteru. Porovnává se každý objekt s každým a nejmenší hodnota je výsledná hodnota
*@pre c1 nesmí být nulový pointer, jinak se program ukončí
*@pre velikost c1 musí být větší než 0, jinak se program ukončí
*@pre c2 nesmí být nulový pointer, jinak se program ukončí
*@pre velikost c2 musí být větší než 0, jinak se program ukončí
*@post změří se nejmenší vdálenost, která výjde z počítaní vzdáleností jednotlivých objektů v clusterech c1 a c2... Porovnává se každý s každým
*@return vrátí se vzdálenost dvou zadaných clusterů v datovém typu "float"
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/*! Funkce najde dva nejbližší clustery v poli clusterů carr
 *@ingroup distance_operations
 *@param carr Pole clusterů, ve kterém se mají najít 2 nejbližší sousedi
 *@param narr počet clusterů, které obsahuje pole clusterů carr
 *@param c1 číselná hodnota, která bude obsahovat index prvního ze dvou nejbližších clusterů v poli clusterů
 *@param c2 číselná hodnota, která bude obsahovat index druhého ze dvou nejbližších clusterů v poli clusterů
 *@pre hodnota narr musí být větší než 0, jinak se program ukončí. To znamená, že pole clusterů obsahuje aspoň jeden cluster
 *@pre hodnota c1 nebude mít na počátku funkce přidělenou hodnotu. Hodnota se musí deklarovat před voláním funkce... Jedná se o pointer
 *@pre hodnota c2 nebude mít na počátku funkce přidělenou hodnotu. Hodnota se musí deklarovat před voláním funkce... Jedná se o pointer
 *@post Pole clusterů carr se nijak nezmění
 *@post hodnota velikosti pole clusterů narr se nijak nezmění
 *@post proměnné c1 se přidělí hodnota indexu prvního ze sousedících clusterů v poli clusterů carr
 *@post proměnné c2 se přidělí hodnota indexu druhého ze sousedících clusterů v poli clusterů carr
 *@return funkce nevrací žádné hodnoty
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/*! Funkce třídí objekty v clusteru vzestupně, podle jejich ID
 *@ingroup cluster_memory_operations
 *@param c Jedná se o cluster, ve kterém mají být objekty setřízeny
 *@pre cluster c by neměl být prázdný
 *@pre ID objektů v clusteru by se nemělo opakovat a nemělo by být prázdné
 *@post po skončení funkce budou objekty v clusteru vytřízeny vzestupně podle jejich ID
 *@return funkce nevrací žádné hodnoty
 */
void sort_cluster(struct cluster_t *c);

/*! Funkce tiskne objekty z jednoho clusteru
 *@ingroup iostream
 *@param c jedná se o cluster, ve kterém se objevují objekty, které se budou tisknout
 *@pre cluster by měl obsahovat nějaké objekty
 *@pre Objekty by měly mít všechny 3 hodnoty definované
 *@post funkce postupně vytiskne všechny objekty, které se v clusteru vyskytují ve formátu ID[x,y]
 *@post Na konci funkce se vytiskne netisknutelný znak konce řádku
 *@return funkce nevrací žádné hodnoty
 */
void print_cluster(struct cluster_t *c);
/*! Funkce načítá a inicializuje objekty a přidává je do clusterů čtením ze vstupního souboru. Pro každý objekt se vytvoří cluster, do kterého se objekt přidá. Clustery se poté vloží do pole clusterů
 *@ingroup iostream
 *@param filename Jedná se o název souboru, ze kterého se budou načítat data. První řádek má podobu "count=N", kde N je číslo vyjadřující počet řádků, se kterýma se má pracovat. Další řádky obsahují data o objektech ve formátu ID X Y
 *@param arr Jedná se o pole clusterů, do kterého se budou ukládat jednotlivé clustery
 *@pre Soubor filename by měl jít otevřít, jinak program vyhodí chybu a vypne se
 *@pre Pole clusterů arr by mělo být definované už před začátkem funkce
 *@post Na konci funkce bude pole clusterů obsahovat všechny vytvořené clustery, které budou obsahovat všechny načtené objekty
 *@post Načte se jen takový počet řádků, jaká je hodnota čísla na prvním řádku. Každý cluster bude obsahovat jeden objekt
 *@return Fuknce vrátí hodnotu vyjadřující počet načtených objektů v datovém typu "int"
 */
int load_clusters(char *filename, struct cluster_t **arr);

/*! Funkce vytiskne všechny objekty podle rozřazení do clusterů oddělené koncem řádku
 *@ingroup iostream
 *@param carr pole clusterů, ze kterého se bude tisknout
 *@param narr počet clusterů, které se mají vytisknout
 *@pre ukazatel carr by neměl být nulový
 *@pre hodnota narr by neměla být menší než 0
 *@post funkce opakovaně volá funkci print_cluster. Vyvolá ji tolikrát, jaká je hodnota proměnné narr
 *@post pole clusterů carr, ani hodnota narr se nezmění
 *@return funkce nevrací žádné hodnoty
 */
void print_clusters(struct cluster_t *carr, int narr);