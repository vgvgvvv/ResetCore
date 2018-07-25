#ifndef __APPLICATION_CONTEXT_H__
#define __APPLICATION_CONTEXT_H__

namespace ReD3D
{
	class ApplicationContext
	{
	public:
		template<typename T>
		static T Get();

		template<typename T>
		static void Regist(T target);
	};
}

#endif //!__APPLICATION_CONTEXT_H__
