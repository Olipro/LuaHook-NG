#pragma once

namespace Olipro {
	template <typename T>
	class CyclicFuture : public std::future<T> {
		std::function<T()> fc;
	public:
		class AsyncResult {
			const std::function<T()>& func;
			std::future<T>& fut;
			bool movedFrom = false;
			AsyncResult(std::function<T()>& func, std::future<T>& fut) :
				func{ func }, fut{ fut }
			{

			}
			friend class CyclicFuture;
		public:
			~AsyncResult() {
				fut = std::async(std::launch::async, func);
			}
			T get() {
				return fut.get();
			}
		};
		CyclicFuture() { };

		template <class Func, class ...Args>
		CyclicFuture(Func f, Args... args) : std::future<T>(std::async(
			std::launch::async, std::bind(f, args...))),
			fc{ std::bind(f, args...) }
		{

		}

		CyclicFuture& operator=(CyclicFuture&& other)
		{
			fc = std::move(other.fc);
			std::future<T>::operator=(std::move(other));
			return *this;
		}

		AsyncResult get() {
			return {fc, *this};
		}
	};
}
