#include <boost/thread.hpp>
#include <boost/lockfree.hpp>

template<typename Data>
class ConcurrentQueue
{
private:
	boost::lockfree::queue<Data> m_queue;
	mutable boost::mutex m_mutex;
	boost::condition_variable m_condVariable;

public:
	void push(Data const& data)
	{
		m_queue.push(data);
		m_condVariable.notify_one();
	}

	void wait_for_pop(Data& popped_value)
	{
		while (!m_queue.pop(popped_value)) {
			boost::unique_lock<boost::mutex> lock(m_mutex);
			m_condVariable.wait(lock);
		}
	}
};