# Grading Execution Time Variance Test

## Running the test

1. SSH to the control host and make sure your user's file descriptor limits are sufficient.
```
# Check system-wide file descriptor limit
sudo cat /proc/sys/fs/file-max
# If low, set to 1M
sudo sysctl -w fs.file-max=1000000

# Check current user's file descriptor limit
ulimit -n
# If low, set to 1M
ulimit -n 1000000

# Note that these limit updates are applied only to the current session.
```
2. Copy `test-dump-gv.tar.gz`, `partition-x10.cpp`, `setup.sh` and `locustfile.py` to the control host.
3. Run `cmsDumpImporter --drop test-dump-gv.tar.gz` to replace the current DB contents with the test data. WARNING: this will delete existing DB data.
4. Run `cmsAddAdmin -p thisisanadminpassword cmsadmin` to create a CMS admin account for monitoring.
5. In the same directory, run `locust` to start serving the Locust web UI.
6. Open the Locust web UI in the browser. Enter the desired number of parallel users (max 1000, recommended 20), spawn rate and CMS contest host, then run the test. Each user will submit 50 times at 2s interval.
7. Monitor the test from the Locust web UI and CMS admin interface. Make sure all submissions are submitted and scored correctly with no errors.

## Analyzing the test results

To get the test results, query the CMS DB using pgAdmin query tool:
```
SELECT * FROM (
	SELECT
		submission_id, compilation_time, evaluation_outcome, score, score_details,
		t.name AS task_name, s.language, s.participation_id,
		(score_details->0->'testcases'->0->'idx') AS tc_name,
		(score_details->0->'testcases'->0->'memory')::float AS tc_memory,
		(score_details->0->'testcases'->0->'time')::float AS tc_time
	FROM public.submission_results AS sr
	LEFT JOIN public.submissions AS s ON sr.submission_id = s.id
	LEFT JOIN public.tasks AS t ON s.task_id = t.id
	WHERE t.name = 'partition'
) AS res
WHERE tc_time IS NOT NULL
ORDER BY submission_id
```

Check whether the range between the minimum and maximum values of `tc_time` is acceptable (generally this range should not be greater than 10%, or around 5% deviation from the mean).

## Updating the test dump

If you modify any CMS contest or task config, you can regenerate the test dump by running this command on the control host, then downloading the generated file.
```
cmsDumpExporter --contest-id 1 --no-generated --no-submissions --no-user-tests --no-print-jobs test-dump-gv.tar.gz
```
