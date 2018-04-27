#pragma once

#include <mutex>
#include <thread>
#include <array>
#include <atomic>
#include <functional>
#include <condition_variable>
#include <queue>

  // While the delegate type can be changed, it must not take parameters.
  // Currently the thread pool does not support passing parameters to the delegate.
template < unsigned NumThreads = 4u, typename DelegateType = std::function<void( void )> >
class ThreadPool {
public:
  // public meta-data
  using job_type = DelegateType;
  static const unsigned thread_count = NumThreads;

  // Interface
  ThreadPool( );
  ~ThreadPool( );
  inline unsigned size( ) const;
  inline unsigned jobs_remaining( );
  void add_job( DelegateType function );
  // Blocks until the current (all if passed true) jobs are completed, then joins all threads.
  void join_threads( bool complete_jobs = true );
  // blocks until every job has completed, doesn't join the threads.
  void wait_all( );

private:
  void work( );
  DelegateType next_job( );
  std::array<std::thread, NumThreads> m_threads;
  std::queue<DelegateType>            m_jobs;
  std::atomic_uint                    m_jobs_remaining;
  std::atomic_bool                    m_abort;
  std::atomic_bool                    m_finished;
  std::condition_variable             m_jobs_available_cv;
  std::condition_variable             m_wait_cv;
  std::mutex                          m_wait_mutex;
  std::mutex                          m_queue_mutex;
};


template< unsigned NumThreads, typename DelegateType >
ThreadPool< NumThreads, DelegateType >::ThreadPool( ) {
  m_jobs_remaining.store( 0u );
  m_abort.store( false );
  m_finished.store( false );

  for( unsigned i{ 0 }; i < NumThreads; ++i ) {
    m_threads[ i ] = std::move( std::thread( [ this, i ] { this->work( ); } ) );
  }
}

/*
* Completes all jobs and joins all threads on destruction.
*/
template< unsigned NumThreads, typename DelegateType >
ThreadPool< NumThreads, DelegateType >::~ThreadPool( ) {
  join_threads( );
}

/*
* Gets the number of threads in this pool
*/
template< unsigned NumThreads, typename DelegateType >
inline unsigned ThreadPool< NumThreads, DelegateType >::size( ) const {
  return NumThreads;
}

/*
* Gets the number of jobs in this pool
*/
template< unsigned NumThreads, typename DelegateType >
inline unsigned ThreadPool< NumThreads, DelegateType >::jobs_remaining( ) {
  std::lock_guard< std::mutex > lock( m_queue_mutex );
  return m_jobs.size( );
}

/*
* Adds a job to the job pool.
* If the queue is empty: Thread is woken and assigned the job.
* If all threads are busy: Job is added to the end of the queue.
*/
template< unsigned NumThreads, typename DelegateType >
void ThreadPool< NumThreads, DelegateType >::add_job( DelegateType job ) {
  std::lock_guard< std::mutex > lock( m_queue_mutex );
  m_jobs.emplace( job );
  ++m_jobs_remaining;
  m_jobs_available_cv.notify_one( );
}

/*
* Joins all of the threads in the pool.
* After this function is completed, the pool is no longer usable.
* See ThreadPool::wait_all() if you need the pool to continue working afterwards.
* Param: complete_jobs: true - pool will complete all jobs in the queue.
                        false - threads will complete and not acquire any new jobs.
*/
template< unsigned NumThreads, typename DelegateType >
void ThreadPool< NumThreads, DelegateType >::join_threads( bool complete_jobs ) {
  if( !m_finished ) {
    if( complete_jobs )
      wait_all( );
    // All jobs should be completed at this point.
    m_abort = true;
    // Wake all of the threads
    m_jobs_available_cv.notify_all( );
    // Join all the threads.
    for( auto &thread : m_threads )
      if( thread.joinable( ) )
        thread.join( );
    m_finished = true;
  }
}

/*
* Waits for the pool to complete all of it's assigned jobs.
* Doesn't join the threads after completion.
*/
template< unsigned NumThreads, typename DelegateType >
void ThreadPool< NumThreads, DelegateType >::wait_all( ) {
  if( m_jobs_remaining > 0u ) {
    // This mutex is used to make sure there aren't multiple threads completing
    std::unique_lock<std::mutex> lock( m_wait_mutex );
    // release the context until all of the jobs have been completed.
    m_wait_cv.wait( lock, [ this ] { return ( this->m_jobs_remaining == 0 ); } );
  }
}

/*
* While the threadpool has jobs in it, execute the next job in the pool.
* Every time a job is completed the main thread is notified.
*/
template< unsigned NumThreads, typename DelegateType >
void ThreadPool< NumThreads, DelegateType >::work( ) {
  while( !m_abort ) {
    // Acquires and executes the next job.
    next_job( )( );
    --m_jobs_remaining;
    m_wait_cv.notify_one( );
  }
}

/*
* Gets the next job and removes it from the job queue.
* Waits for a signal from main thread if no jobs exist.
*/
template< unsigned NumThreads, typename DelegateType >
DelegateType ThreadPool< NumThreads, DelegateType >::next_job( ) {
  DelegateType job;
  std::unique_lock<std::mutex> lock( m_queue_mutex );

  // wait for a job
  m_jobs_available_cv.wait( lock, [ this ] { return ( m_jobs.size( ) || m_abort ); } );

  // get a job
  if( !m_abort ) {
    job = m_jobs.front( );
    m_jobs.pop( );
  } else // When aborting, return a fake job to maintain m_jobs_remaining accuracy.
  {
    job = [] { };
    ++m_jobs_remaining;
  }
  return job;
}