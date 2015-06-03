// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/scheduler/test/lazy_scheduler_message_loop_delegate_for_tests.h"

namespace scheduler {

// static
scoped_refptr<LazySchedulerMessageLoopDelegateForTests>
LazySchedulerMessageLoopDelegateForTests::Create() {
  return make_scoped_refptr(new LazySchedulerMessageLoopDelegateForTests());
}

LazySchedulerMessageLoopDelegateForTests::
    LazySchedulerMessageLoopDelegateForTests()
    : thread_id_(base::PlatformThread::CurrentId()) {
}

LazySchedulerMessageLoopDelegateForTests::
    ~LazySchedulerMessageLoopDelegateForTests() {
}

base::MessageLoop* LazySchedulerMessageLoopDelegateForTests::EnsureMessageLoop()
    const {
  base::MessageLoop* message_loop = base::MessageLoop::current();
  DCHECK(message_loop);
  for (auto& observer : pending_observers_) {
    message_loop->AddTaskObserver(observer);
  }
  pending_observers_.clear();
  return message_loop;
}

bool LazySchedulerMessageLoopDelegateForTests::HasMessageLoop() const {
  return base::MessageLoop::current() != nullptr;
}

bool LazySchedulerMessageLoopDelegateForTests::PostDelayedTask(
    const tracked_objects::Location& from_here,
    const base::Closure& task,
    base::TimeDelta delay) {
  return EnsureMessageLoop()->task_runner()->PostDelayedTask(from_here, task,
                                                             delay);
}

bool LazySchedulerMessageLoopDelegateForTests::PostNonNestableDelayedTask(
    const tracked_objects::Location& from_here,
    const base::Closure& task,
    base::TimeDelta delay) {
  return EnsureMessageLoop()->task_runner()->PostNonNestableDelayedTask(
      from_here, task, delay);
}

bool LazySchedulerMessageLoopDelegateForTests::RunsTasksOnCurrentThread()
    const {
  return thread_id_ == base::PlatformThread::CurrentId();
}

bool LazySchedulerMessageLoopDelegateForTests::IsNested() const {
  return EnsureMessageLoop()->IsNested();
}

void LazySchedulerMessageLoopDelegateForTests::AddTaskObserver(
    base::MessageLoop::TaskObserver* task_observer) {
  if (!HasMessageLoop()) {
    pending_observers_.insert(task_observer);
    return;
  }
  EnsureMessageLoop()->AddTaskObserver(task_observer);
}

void LazySchedulerMessageLoopDelegateForTests::RemoveTaskObserver(
    base::MessageLoop::TaskObserver* task_observer) {
  if (!HasMessageLoop()) {
    pending_observers_.erase(task_observer);
    return;
  }
  EnsureMessageLoop()->RemoveTaskObserver(task_observer);
}

}  // namespace scheduler
