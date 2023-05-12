#pragma once


namespace olc
{
	template <typename T>
	class singleton : public T
	{
	protected:
		singleton() {}

		singleton(const singleton&) = delete;
		singleton& operator= (const singleton&) = delete;

	public:
		static singleton<T>& instance()
		{
			static singleton<T> obj;
			create_object.do_nothing();
			return obj;
		}

	private:
		class object_creator
		{
		public:
			object_creator() { singleton<T>::instance(); }
			inline void do_nothing() const {}
		};
		static object_creator create_object;
	};

	template <typename T>
	typename singleton<T>::object_creator singleton<T>::create_object;
}
