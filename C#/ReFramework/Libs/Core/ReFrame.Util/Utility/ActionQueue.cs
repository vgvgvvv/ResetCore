using System.Collections;
using System.Collections.Generic;
using System;

namespace ResetCore.Util
{
    /// <summary>
    /// 即时触发式行为队列，入队的同时就开始调用
    /// </summary>
    public class ActionQueue
    {

        //队列中是否有行为
        private bool isLoading = false;
        //行动队列
        private Queue<Action> m_queue = new Queue<Action>();

        public ActionQueue()
        {
            isLoading = false;
        }

        #region 主要逻辑
        /// <summary>
        /// 插入行为，用户可以控制下一个行为在该的行为中调用的位置。
        /// </summary>
        /// <param name="actionCB"></param>
        /// <returns></returns>
        public ActionQueue AddAction(Action<Action> actionCB)
        {
            if (isLoading)
            {
                //队列非空，则加入队列
                this.m_queue.Enqueue(() =>
                {
                    this.WaitAndDo(actionCB);
                });
            }
            else
            {
                //现在正在进行调用isLoading为true
                isLoading = true;
                //队列为空，直接调用，但是下一个行为在运行时需要加入队列
                this.WaitAndDo(actionCB);
            }
            return this;
        }

        private void WaitAndDo(Action<Action> actionCB)
        {
            //在行为调用完成之后进行回调
            actionCB(TryDequeue);
        }

        private void TryDequeue()
        {
            if (m_queue.Count != 0)
            {
                //出队列得到Action并且调用
                m_queue.Dequeue().Invoke();
            }
            else
            {
                //表示队列为空，下次可以直接进行调用
                isLoading = false;
            }
        }

        /// <summary>
        /// 插入行为（下个动作流程自动加到该动作的后面）
        /// </summary>
        /// <param name="actionCB"></param>
        /// <returns></returns>
        public ActionQueue AddAction(Action actionCB)
        {
            if (isLoading)
            {
                //队列非空，则加入队列
                this.m_queue.Enqueue(() =>
                {
                    this.WaitAndDo(actionCB, this.m_queue);
                });
            }
            else
            {
                //现在正在进行调用isLoading为true
                isLoading = true;
                //队列为空，直接调用，但是下一个行为在运行时需要加入队列
                this.WaitAndDo(actionCB, this.m_queue);
            }
            return this;
        }

        private void WaitAndDo(Action actionCB, Queue<Action> q)
        {
            //在行为调用完成之后进行回调
            actionCB();
            TryDequeue();
        }

        #endregion

        

        public void Clean()
        {
            m_queue.Clear();
            m_queue = new Queue<Action>();
            isLoading = false;
        }
    }
}
