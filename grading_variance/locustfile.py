import time
import os
import threading
from locust import HttpUser, task, constant
from locust.exception import StopUser

contest_slug = 'toki-cms'
task_slug = 'partition'
submission_file_path = 'partition-x10.cpp'
submission_language = 'C++17 / g++'
submissions_per_user = 50
seconds_between_submissions = 2

user_counter = 0
user_counter_lock = threading.Lock()

class ContestantUser(HttpUser):
    wait_time = constant(seconds_between_submissions)

    def on_start(self):
        with user_counter_lock:
            global user_counter
            user_counter += 1
            self.user_index = user_counter

        self.submission_counter = 0

        resp = self.client.get('/' + contest_slug)
        self.csrf_token = resp.cookies['_xsrf']
        self.client.post('/' + contest_slug + '/login', data={
            '_xsrf': self.csrf_token,
            'username': 'user{:04d}'.format(self.user_index),
            'password': 'password{:04d}'.format(self.user_index),
        })

    @task
    def submit(self):
        if self.submission_counter >= submissions_per_user:
            while True:
                time.sleep(1)

        submission_file_name = os.path.basename(submission_file_path)
        submission_file = open(submission_file_path, 'rb')
        self.client.post('/' + contest_slug + '/tasks/' + task_slug + '/submit', data={
            '_xsrf': self.csrf_token,
            'language': submission_language,
        }, files={
            task_slug + '.%l': (submission_file_name, submission_file),
        })

        self.submission_counter += 1
