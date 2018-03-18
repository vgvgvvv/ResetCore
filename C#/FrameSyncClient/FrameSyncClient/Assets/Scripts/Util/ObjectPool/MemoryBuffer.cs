using System;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.Util
{
    /// <summary>
    /// 内存复用块
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class MemoryBuffer<T>
    {
        private MemoryBufferPool<T> _poolRef = null;
        private T[] _bufRef = null;
        private int _count = 0;

        public bool IsInited { get { return _bufRef != null && _poolRef != null; } }
        public int Count { get { return _count; } }
        public int Capacity { get { return _bufRef != null ? _bufRef.Length : 0; } }
        public T[] Buffer { get { return _bufRef; } }

        public T this[int index]
        {
            get
            {
                if (!IsInited)
                {
                    Debug.LogError("MBuffer is not inited");
                    return default(T);
                }
                if (index < 0 || index >= _count)
                {
                    Debug.LogError("MBuffer set out of range");
                    return default(T);
                }
                return _bufRef[index];
            }
            set
            {
                if (!IsInited)
                {
                    Debug.LogError("MBuffer is not inited");
                    return;
                }
                if (index < 0 || index >= _count)
                {
                    Debug.LogError("MBuffer set out of range");
                    return;
                }
                _bufRef[index] = value;
            }
        }

        public void Init(MemoryBufferPool<T> pool, T[] buffer)
        {
            _poolRef = pool;
            _bufRef = buffer;
        }

        public void Uninit()
        {
            _poolRef = null;
            _bufRef = null;
        }

        public void Add(T value)
        {
            if (!IsInited)
            {
                Debug.LogError("MBuffer is not inited");
                return;
            }

            if (_count == _bufRef.Length)
            {
                T[] bufNew = _poolRef.AllocBuffer(_bufRef.Length << 1);
                Array.Copy(_bufRef, 0, bufNew, 0, _bufRef.Length);
                _poolRef.FreeBuffer(_bufRef);
                _bufRef = bufNew;
            }

            this[_count++] = value;
        }

        public bool Remove(T item)
        {
            if (!IsInited)
            {
                Debug.LogError("MBuffer is not inited");
                return false;
            }

            for (int i = 0; i < _count; i++)
            {
                if (this[i].Equals(item))
                {
                    this[_count - 1] = default(T);
                    for (int j = i; j < _count - 1; j++)
                    {
                        this[j] = this[j + 1];
                    }
                    _count--;
                    return true;
                }
            }

            return false;
        }

        public void RemoveAt(int index)
        {
            if (!IsInited)
            {
                Debug.LogError("MBuffer is not inited");
                return;
            }
            if (index < 0 || index >= _count)
            {
                Debug.LogError("MBuffer set out of range");
                return;
            }
            this[_count - 1] = default(T);
            for (int j = index; j < _count - 1; j++)
            {
                this[j] = this[j + 1];
            }
            _count--;
        }

        public bool Contains(T item)
        {
            if (!IsInited)
            {
                Debug.LogError("MBuffer is not inited");
                return false;
            }
            for (int i = 0; i < _count; ++i)
            {
                if (this[i].Equals(item))
                {
                    return true;
                }
            }
            return false;
        }

        public void Clear()
        {
            _count = 0;
        }

        public void Copy(T[] src, int srcIdx, int length)
        {
            if (src.Length < length - srcIdx)
            {
                Debug.LogError("src buffer out of range");
                return;
            }
            if (Capacity < length)
            {
                Debug.LogError("dest buffer out of range");
                return;
            }
            Array.Copy(src, srcIdx, _bufRef, 0, length);
            _count = length;
        }
    }

    /// <summary>
    /// 内存复用池
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class MemoryBufferPool<T>
    {
        // buffer池，索引idx里存的Queue为size == (1<<idx)的buffer
        private Queue<T[]>[] _bufferPools = new Queue<T[]>[32];

        public void Preload(int size, int count)
        {
            int idx = getPoolIdx(size);
            if (idx < 0)
            {
                Debug.LogErrorFormat("Invalid preload size, size={0}", size);
                return;
            }
            if (_bufferPools[idx] == null)
            {
                _bufferPools[idx] = new Queue<T[]>();
            }
            for (int i = 0; i < count; i++)
            {
                T[] buffer = new T[1 << idx];
                _bufferPools[idx].Enqueue(buffer);
            }
        }

        public MemoryBuffer<T> GetBuffer(int size)
        {
            MemoryBuffer<T> buffer = CommonObjectPool<MemoryBuffer<T>>.Get();
            T[] bufRef = AllocBuffer(size);
            buffer.Init(this, bufRef);
            return buffer;
        }

        public void ReturnBuffer(MemoryBuffer<T> buffer)
        {
            if (!buffer.IsInited)
            {
                Debug.LogError("Duplicate return buffer");
                return;
            }
            FreeBuffer(buffer.Buffer);
            buffer.Uninit();
            CommonObjectPool<MemoryBuffer<T>>.Return(buffer);
        }

        public T[] AllocBuffer(int size)
        {
            int idx = getPoolIdx(size);
#if DEBUG
            if (idx >= 20) // >= 1M
            {
                Debug.LogErrorFormat("alloc too huge size, type={0}, size={1}", typeof(T), size);
            }
            else if (idx >= 15) // >= 32K
            {
                Debug.LogErrorFormat("alloc big size,  type={0}, size={1}", typeof(T), size);
            }
#endif

            T[] bufRef = null;
            Queue<T[]> pool = _bufferPools[idx];
            if (pool != null && pool.Count > 0)
            {
                bufRef = pool.Dequeue();
            }
            if (bufRef == null)
            {
                bufRef = new T[1 << idx];
            }
            return bufRef;
        }

        public void FreeBuffer(T[] buffer)
        {
            int idx = getPoolIdx(buffer.Length);
            Queue<T[]> pool = _bufferPools[idx];
            if (pool == null)
            {
                pool = new Queue<T[]>();
                _bufferPools[idx] = pool;
            }

            Array.Clear(buffer, 0, buffer.Length);
            pool.Enqueue(buffer);
#if DEBUG
            if (pool.Count > (1 << 20) / (1 << idx))
            {
                Debug.LogErrorFormat("too many buffer in pool, type={0}, size={1}, count={2}", typeof(T), 1 << idx, pool.Count);
            }
#endif
        }

        public void ClearPool()
        {
            for (int i = 0; i < _bufferPools.Length; i++)
            {
                if (_bufferPools[i] != null)
                {
                    _bufferPools[i].Clear();
                    _bufferPools[i] = null;
                }
            }
        }

        private int getPoolIdx(int size)
        {
            int idx = -1;
            for (int i = 0; i < _bufferPools.Length; i++)
            {
                if ((1 << i) >= size)
                {
                    idx = i;
                    break;
                }
            }
            return idx;
        }
    }

    /// <summary>
    /// 内存复用管理器
    /// </summary>
    public class MemoryBufferManager : Singleton<MemoryBufferManager>
    {
        private MemoryBufferPool<int> _intBufferPool = new MemoryBufferPool<int>();
        private MemoryBufferPool<uint> _uintBufferPool = new MemoryBufferPool<uint>();
        private MemoryBufferPool<ulong> _ulongBufferPool = new MemoryBufferPool<ulong>();
        private MemoryBufferPool<float> _floatBufferPool = new MemoryBufferPool<float>();
        private MemoryBufferPool<object> _objBufferPool = new MemoryBufferPool<object>();
        private MemoryBufferPool<byte> _byteBufferPool = new MemoryBufferPool<byte>();

        public override void Init()
        {
            // 预先分配内存 TODO
        }

        // int
        public MemoryBuffer<int> GetIntBuffer(int size)
        {
            return _intBufferPool.GetBuffer(size);
        }
        public void ReturnUIntBuffer(MemoryBuffer<int> buffer)
        {
            _intBufferPool.ReturnBuffer(buffer);
        }

        // uint
        public MemoryBuffer<uint> GetUIntBuffer(int size)
        {
            return _uintBufferPool.GetBuffer(size);
        }
        public void ReturnUIntBuffer(MemoryBuffer<uint> buffer)
        {
            _uintBufferPool.ReturnBuffer(buffer);
        }

        // ulong
        public MemoryBuffer<uint> GetULongBuffer(int size)
        {
            return _uintBufferPool.GetBuffer(size);
        }
        public void ReturnULongBuffer(MemoryBuffer<uint> buffer)
        {
            _uintBufferPool.ReturnBuffer(buffer);
        }

        // float
        public MemoryBuffer<float> GetFloatBuffer(int size)
        {
            return _floatBufferPool.GetBuffer(size);
        }
        public void ReturnFloatBuffer(MemoryBuffer<float> buffer)
        {
            _floatBufferPool.ReturnBuffer(buffer);
        }

        // object
        public MemoryBuffer<object> GetObjBuffer(int size)
        {
            return _objBufferPool.GetBuffer(size);
        }
        public void ReturnObjBuffer(MemoryBuffer<object> buffer)
        {
            _objBufferPool.ReturnBuffer(buffer);
        }

        // byte
        public MemoryBuffer<byte> GetByteBuffer(int size)
        {
            return _byteBufferPool.GetBuffer(size);
        }
        public void ReturnByteBuffer(MemoryBuffer<byte> buffer)
        {
            _byteBufferPool.ReturnBuffer(buffer);
        }
    }
}
