using Entity = u32;

const u32 MAX_ENTITIES = 50000;

// A simple type alias
using ComponentType = u8;

// Used to define the size of arrays later on
const ComponentType MAX_COMPONENTS = 64;

using Signature = u32;

u32 livingEntityCount = 0;
Signature signatures [MAX_ENTITIES];
u64 nextEntityId = 0; 

Entity CreateEntity ();
void DestroyEntity (Entity entity);
void SetSignature(Entity entity, Signature signature);
Signature GetSignature(Entity entity);