
// messages, actions and properties are connected
// properties of an item: edible, throwable, droppable, glowable ...
// actions: eat, throw, drop, glow ...
// messages: i eat item, player2 throws item, i drop item, item glows ...

// an attribute is a tuple of { on/off boolean, min/max range, add/mul modifier, vec3 }. Ie,
//  damage { add: 10, mult: 2.0 }         // +10 x2
//  visible { bool: true }                // yes
//  health { min:56, max: 100 }           // range (min == ~cur)
//  position { x: 0, y: 0, z: 0 }         // vec3

// item is a collection of attributes
// - messages are delivered to each attribute field
// - attributes can alter, ignore or cancel message before passing it to owner
// item has globally all attributes of its children

// inventory is a collection of items
// inventory has globally all attributes of its children

// - player
//   + inventory
//     - health
//     -



// player -> inventory -> items -> attributes
// inventory = Σ items
// item = Σ attributes
//
// mechanics:
// sample 1)
//  player equips 'boots'
//  game send 'equip' message to boots in inventory,
//  boots increases 'player.speed' by boots.points
// sample 2)
//  spider attacks player
//  game sends 'attack' message to player's inventory,
//  'shield' equipped item reduces message quantity to -50P
//  'armor' equipped item reduces message quantity to -50%
//  after whole inventory iteration, game decreases player's
//  health by remaining damage if positive.
// sample 3)
//  player burns in fire for some reason,
//  then game sends 'burn' message to player's inventory,
//  which sends forward message to every carried item,
//  which sends forward message to every property,
//  those with 'flammable' attribute will react,
//  and then might morph, spawn or die accordly.
//  ie, paper morphs to paper+fire, that will morph to ashes later on,
//  that will drop from inventory automatically. result: ashes on floor.
//
// weapon: dagger, stick, short sword, axe, spear, long sword, two sword, halberd,
// light source: torch, lamp
// armor: { ugly clothes, plate armor, }
// boots: { light, heavy(armor), speed }
// wand
// mail
// keys
// scroll
// spellbook
// potion: { sanity, poison, hp, mp }
// ring: ~magical/~random { +armor,-speed, } or combo { +silence && +speed }


// ejemplo real del tol
/*
        s32             m_Ammo;                     // Numero de balas en el arma
        s32             m_MaxAmmo;                  // Numero de balas maximo
        s32             m_AmmoPerClip;                  // Numero de balas maximo
        s32             m_Rechargable;              // Si el arma necessita recargarse
        s32             m_TotalAmmo;                // Numero total de balas disponibles
        s32             m_MaxClips;                 // Numero de cargadores maximo
        f32             m_Damage;                   // Daño que causan los proyectiles
        f32             m_HeadshotDamage;           // Daño que causan los proyectiles al impactar en la cabeza
        f32             m_Delay;                    // Tiempo de delay entre que se apreta el gatillo y se dispara
        f32             m_Distance;                 // En el caso de las armas blancas, indica la longitud del arma
        f32             m_RangeAngle;               // En el caso de las armas blancas, indica el angulo que recorre el arma al atacar (radians)
        f32             m_MinRadius;                // En el caso de las armas explosivas, indica la distancia de explosión hasta donde el efecto es máximo
        f32             m_MaxRadius;                // En el caso de las armas explosivas, indica la distancia hasta donde la explosión hace efecto
        f32             m_NoAimDamageModif;         // Modificador del Daño que causan los proyectiles sin apuntado fino
*/

