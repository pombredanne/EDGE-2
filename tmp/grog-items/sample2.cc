
struct item2
{
    struct {
    int max = 0;        // 9 turns
    int current = 0;    // 7 turns
    } timeout;

    struct {
    bool autoch = 0;    // yes/no
    int max = 0;        // 9 turns
    int current = 0;    // 7 turns
    } charges;

    struct {
    int min = 0;        // 3 bullets
    int max = 0;        // 8 bullets
    } strikes;

    int spawns[4];  // when spawning, spaws into
    int breaks[4];  // when dying, breaks into

    item2() {
        for( int i = 0; i < 4; ++i ) {
            spawns[i] = breaks[i] = 0;
        }
    }
};
//#include <moon9/play/tree.hpp>
//typedef moon9::tree<size_t> scenetree;

int main2( void )
{
    int root = id();
    int options = id();
    int battleground = id();
    int player1 = id();
    int player2 = id();
    int obj1 = id();
    int obj2 = id();

    int speed = id();
    int attack = id();

    //scenetree scene( root );

    std::map< size_t /*id*/, ::attribute > attribute;
    attribute[ speed ] = attribute[ speed ];
    attribute[ attack ] = attribute[ attack ];

    {
        auto &with = attribute[ speed ];
        with.name = "speed";
        with.add = 10;
        with.handle = 0;
    }

    {
        auto &with = attribute[ attack ];
        with.name = "attack";
        with.add = 100;
        with.handle = 0;
    }

    std::map< size_t /*id*/, ::player > player;
    player[ player1 ] = player[ player1 ];
    player[ player2 ] = player[ player2 ];

    std::map< size_t /*id*/, ::item > item;
    item[ obj1 ] = item[ obj1 ];
    item[ obj2 ] = item[ obj2 ];

    /*
    scene.
    node( options ).
    push( battleground ).
        push( player1 ).
            node( speed ).
            node( attack ).
            back().
        push( player2 ).
            node( speed ).
            node( attack ).
            push( obj1 ).
                node( attack ).
                back().
            node( obj2 ).
            back();

    int sum = 0;
    //scene.apply( [&sum](int &x){ sum += x; } );
    std::cout << scene.str() << std::endl;
    */

    std::cout << std::fixed << player[player1].speed() << std::endl;

    return 0;
}
