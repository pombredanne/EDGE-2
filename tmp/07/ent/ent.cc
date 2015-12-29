#include <map>
#include <typeinfo>
#include <set>

typedef size_t EntityID;
struct BaseComponent
{
 	virtual ~BaseComponent(){};
	EntityID BelongsTo;
};

template<typename T>
struct DerivedComponent : public BaseComponent
{
	template<typename ... Args> DerivedComponent(Args... args) : mData(args...) {};
	template<typename O> DerivedComponent<T>& operator=(const O& o) { mData = o; return *this; };
	operator const T&() const { return mData; };
	operator T&(){ return mData; };
private:
	T mData;
};

std::map<std::string, std::set<BaseComponent*> >& _AllComponents()
{ 
	static std::map<std::string, std::set<BaseComponent*> > all;
	return all;
};

template<typename T> std::string GenerateName(){ return typeid(T).name(); };

struct MetaData
{
	EntityID ID;
	std::map<std::string, BaseComponent*> Components;
	
	template<typename T>
	DerivedComponent<T>& get(const std::string& name = GenerateName<T>())
	{
		auto& comp = Components[name];
		if(!comp){ comp = new DerivedComponent<T>(); comp->BelongsTo = ID; _AllComponents()[name].insert(comp); }
		return  *dynamic_cast<DerivedComponent<T>*>(comp);
	};
	
	template<typename T=void>
	void remove(const std::string& name = GenerateName<T>())
	{
		auto it = Components.find(name);
		if(it != Components.end())
		{
			delete it->second;
			_AllComponents()[name].erase(it->second);
			Components.erase(it);
		}
	};
	
	template<typename T=void>
	bool has(const std::string& name = GenerateName<T>())
	{
		return Components.count(name);
	};
};

std::map<EntityID, MetaData>& AllEntities()
{
	static std::map<EntityID, MetaData> all;
	return all;
};

MetaData& Entity(EntityID id)
{
	auto& meta = AllEntities()[id];
	meta.ID = id;
	return meta;
};

template<typename T>
const std::set<DerivedComponent<T>*>& AllComponents(const std::string& name = GenerateName<T>())
{ 
	return *reinterpret_cast<std::set<DerivedComponent<T>*>*>(&(_AllComponents()[name])); // Treat the set like it contains DerivedComponent<T> so it doesn't needlessy copies the whole set
};
// END CODE

// START EXAMPLES

typedef int Property;
typedef float Karma;
typedef std::pair<int, int> Position;

#include <cassert>

int main()
{
	int Player=0, Enemy=1;
	
	Entity(Player).get<Property>("Health") = 100;
	Entity(Player).get<Property>("Mana") = 200;
	Entity(Player).get<Karma>() = 1.0f;
	Entity(Player).get<Position>() = std::make_pair(10, 10);
	Entity(Player).remove<Position>();
	
	Entity(Enemy).get<Property>("Health") = 200;
	Entity(Enemy).get<Karma>() = -1.0f;
	
	// Let's try to murder everyone.
	for(auto health : AllComponents<Property>("Health"))
		(*health) -= 50;
		
	// Let all Entities reach Universal Peace
	for(auto karma : AllComponents<Karma>())
		(*karma) = 0; 
	
	assert(Entity(Player).get<Property>("Health") == 50);
	assert(Entity(Player).get<Property>("Mana") == 200);
	assert(Entity(Enemy).get<Property>("Health") == 150);
	assert(!Entity(Player).has<Property>());
	assert(Entity(Enemy).get<Karma>() == 0);
	assert(Entity(Player).get<Karma>() == 0);
	return 0;
}