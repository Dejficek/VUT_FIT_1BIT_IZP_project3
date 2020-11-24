/************************************/
/* * *	  VUT/FIT/IZP/proj3.c	* * */
/* * *							* * */
/* * *		  verze: 1			* * */
/* * *							* * */
/* * *		  David Rubý		* * */
/* * *		prosinec 2018		* * */
/************************************/

/**
 * Kostra programu pro 3. projekt IZP 2018/19
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <errno.h>
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

void help(){
    printf("\n==help==\n"
			"A program making cluster analysis.\n"
			"\nhow to use:\n"
            "\n./proj3 file [N]\n"
            "file...\t\ta name of a file, where your data are stored.\n"
            "[N]...\t\ta number of total number of clusters, you want to achieve.\n");
}

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);


    //Pokud je kapacita větší než 0, alokuje se paměť pro část objektu a zvětší se proměnná pro kapacitu.
    if (cap > 0){
        c->obj = malloc(sizeof(struct obj_t) * cap);
        if(c->obj != NULL){
            c->capacity = cap;
        }
    }

   
    //Jinak se kapacita nastaví na 0 a objekt na NULL.
    else{
        c->capacity = 0;
        c->obj = NULL;
    }
    c->size = 0;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    //Uvolní místo pro objekty a inicializuje se prázdný cluster.
    free(c->obj);
    init_cluster(c, 0);
}

//Uvolňuje veškerou použitou paměť.
void free_memory(struct cluster_t *clusters, int number_of_objects){
    for(int i = 0; i < number_of_objects; i++){
        clear_cluster(&clusters[i]);
    }
    //free(clusters);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    resize_cluster(c, c->capacity + 1);
    c->obj[c->size++] = obj;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    for(int i = 0; i < c2->size; i++){
        append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    clear_cluster(&carr[idx]);

    for(int i = idx; i < narr - 1 ; i++){
        carr[i] = carr[i + 1];
    }
    return narr - 1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    return sqrtf(pow(o1->x - o2->x, 2) + pow(o1->y - o2->y, 2));
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float min = 2000, distance;

    //Porovnává každý objekt s každým.
    for(int i = 0; i < c1->size; i++){
        for(int j = 0; j < c2->size; j++){
            distance = obj_distance(&c1->obj[i], &c2->obj[j]);
            if(distance < min){
                min = distance;
            }
        }
    }
    return min;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    float min = 2000, distance;
    for(int i = 0; i < narr; i++){
        for(int j = 0; j < narr; j++){
            distance = cluster_distance(&carr[i], &carr[j]);
            if(distance < min && i != j){
                *c1 = i;
                *c2 = j;
                min = distance;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    //Otevření souboru.
    FILE *soubor = fopen(filename, "r");
    if(soubor == NULL){
        fprintf(stderr, "\nWarning. An error occurred while loading a file. %s. Program will be terminated.\n", strerror(errno));
		help();
		exit(1);
    }

    int count, id = -1, line_number = 0, ids[10000], x = -1, y = -1;
    char line[100];// s_id[1000], s_x[1000], s_y[1000];

    //Načítení prvního řádku a extrahování hodnoty COUNT.
    fgets(line, 100, soubor);

    //Kontrola tvaru prvního řádku.
    if(sscanf(line, "count=%d", &count) < 1){
        if(soubor != NULL){
            fclose(soubor);
        }
        fprintf(stderr, "\nWarning. Invalid 1st line of the file.\n");
        help();
        exit(1); 
    }

    if(count <= 0){
        if(soubor != NULL){
            fclose(soubor);
        }
        fprintf(stderr, "\nWarning. Count can not be a negative number.\n");
        help();
        exit(1);
    }

    struct cluster_t *clusters;

    //Alokování paměti pro pole clusterů.
    clusters = malloc(count * sizeof(struct cluster_t));

    //Načítaní ostatních řádků, separování na jednotlivé data.
    while(fgets(line, 100, soubor)){

        //Ošetření proti nepovoleným znakům v souboru.
        for(unsigned int i = 0; i < strlen(line); i++){
            if((line[i] < 48 || line[i] > 57) && line[i] != '\n' && line[i] != '\r' && line[i] != '\0' && line[i] != ' '){
                free_memory(clusters, line_number);
                free(clusters);
                fclose(soubor);
                fprintf(stderr, "\nWarning. ID, X and Y coordinates must be a number.\n");
                help();
                exit(1);
            }
        }

        //převedení dat id, x a y na datové typy int, float a float.
        sscanf(line, "%d %d %d", &id, &x, &y);

        if(id == -1 || x == -1 || y == -1){
            free_memory(clusters, line_number);
            free(clusters);
            fclose(soubor);
            fprintf(stderr, "\nWarning. There must be 3 rows of data in the file.\n");
            help();
            exit(1);
        }


        //Kontrola rozměrů souřadnicového systému.
        if(x < 0 || x > 1000){
            free_memory(clusters, line_number);
            free(clusters);
            fclose(soubor);
            fprintf(stderr, "\nWarning. Object %d must have x coordinate between 0 and 1000.\n", id);
            help();
            exit(1);
        }
        if(y < 0 || y > 1000){
            free_memory(clusters, line_number);
            free(clusters);
            fclose(soubor);
            fprintf(stderr, "\nWarning. Object %d must have y coordinate between 0 and 1000.\n", id);
            help();
            exit(1);
        }

        //Kontrola duplicitních id.
        for(int j = 0; j < line_number; j++){
            if(id == ids[j]){
                free_memory(clusters, line_number);
                free(clusters);
                fclose(soubor);
                fprintf(stderr, "\nWarning. Two objects have the same id... Id = %d\n", id);
                help();
                exit(1);
            }
        }
        ids[line_number] = id;

        //Deklarace objektu a přidělení hodnot.
        struct obj_t obj;
        obj.id = id;
        obj.x = (float)x;
        obj.y = (float)y;

        //Deklarace clusteru, inicializace, přidání pointru na objekt a přidání pointru na cluster do pole clusterů.
        struct cluster_t cluster;
        init_cluster(&cluster, 1);
        append_cluster(&cluster, obj);
        clusters[line_number] = cluster;
        line_number++;

        
        //Opatření, když se ve stupním souboru nachází více řádků, než je uvedeno na prvním řádku.
        if(line_number == count){
            break;
        }
        x = -1;
        y = -1;
        id = -1;
    }
    if(count > line_number){
            free_memory(clusters, line_number);
            free(clusters);
            fclose(soubor);
            fprintf(stderr, "\nWarning. count=%d is higher number, than number of lines.\n", count);
            help();
            exit(1);
        }
    *arr = clusters;

    //Uzavření souboru.
    if(soubor != NULL){
        fclose(soubor);
    }

    //Vrací počet objektů.
    return count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    int argument;


    if(argc == 2){
        argument = 1;
    }
    else{
        argument = atoi(argv[2]);
    }
    
    
    

    struct cluster_t *clusters;

    //Kontrola počtu argumentů.
    if(argc < 2 || argc > 3){
        fprintf(stderr, "\nWarning, You must input 2 or 3 arguments.\n");
        help();
        return 1;
    }

    //Pokud je některý z argumentů "--help", "-help", "--h" nebo "-h", vypíše se nápověda.
	for(int i = 0; i < argc; i++){
		if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--h") == 0 || strcmp(argv[i], "-h") == 0){
			help();
			return 0;
		}
	}

    //Kontrola, zda je druhý argument v číselné podobě.
    if(argc == 3){
        for(unsigned int i = 0; i < strlen(argv[2]); i++){
            if(argv[2][i] < 48 || argv[2][i] > 57){
                fprintf(stderr, "\nWarning. The second argument must be a number or must be positive.\n");
                help();
                return 1;
            }
        }
    }
    
    int total_number_of_objects = argument, i, j;

    if(total_number_of_objects <= 0){
        fprintf(stderr, "\nWarning. No clusters.\n");
        return 1;
    }

    //Načtení clusterů.
    int number_of_objects = load_clusters(argv[1], &clusters);

    if(total_number_of_objects > number_of_objects){
        free_memory(clusters, number_of_objects);
        free(clusters);
        fprintf(stderr, "\nWarning. The second argument has bigger value, total number of objects in the file.\n");
        help();
        return 1;
    }

    if(number_of_objects == -1){
        return 1;       
    }

    //Spojování clusterů.
    while(number_of_objects != total_number_of_objects){
        find_neighbours(clusters, number_of_objects, &i, &j);
        merge_clusters(&clusters[i], &clusters[j]);
        number_of_objects = remove_cluster(clusters, number_of_objects, j);
    }

    print_clusters(clusters, total_number_of_objects);


    //Uvolňuje paměť všwm zbytkům paměti.
    free_memory(clusters, number_of_objects);
    free(clusters);
return 0;
}