using System.Collections.Generic;
using System.Security.Cryptography.X509Certificates;

namespace ReFrame.Core
{
    public class REntityManager : RManager
    {
        private static ulong currentUID = 0;
        private static Dictionary<ulong, REntity> _entities;
        private static List<ulong> delayDestroyEntities;

        public REntityManager()
        {
            _entities = new Dictionary<ulong, REntity>();
            delayDestroyEntities = new List<ulong>();
        }
        
        /// <summary>
        /// 添加新的Entity
        /// </summary>
        /// <param name="entity"></param>
        /// <typeparam name="T"></typeparam>
        public static void AddEntity<T>(T entity) where T:REntity, new()
        {
            _entities.Add(currentUID, entity);
            currentUID++;
        }
        
        public override void Init()
        {
            base.Init();
            _entities.Clear();
            delayDestroyEntities.Clear();
        }

        public override void Update()
        {
            base.Update();

            for (int i = 0; i < delayDestroyEntities.Count; i++)
            {
                _entities.Remove(delayDestroyEntities[i]);
            }
            delayDestroyEntities.Clear();
            
            foreach (var kvp in _entities)
            {
                var entity = kvp.Value;
                if(kvp.Value.Destroyed)
                    delayDestroyEntities.Add(entity.UID);
                else
                    entity.Update();
            }
        }

        public override void Uninit()
        {
            base.Uninit();
            foreach (var entitiesValue in _entities.Values)
            {
                entitiesValue.Destroy();
            }
            _entities.Clear();
            delayDestroyEntities.Clear();
        }
    }
}