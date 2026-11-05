### Design Decision

For the weather station pipeline, both pipes and shared memory can support communication between the sensor-reading and analysis processes.

A pipe is suitable for a simple producer-consumer design because it provides a straightforward communication channel and automatically handles blocking when data is unavailable. Closing the write end signals EOF to the receiving process.

Shared memory is more suitable when large amounts of data must be exchanged frequently. It avoids repeated kernel-mediated data transfer, but requires explicit synchronisation using semaphores to prevent race conditions.

Therefore, a pipe is recommended for the basic weather-reading pipeline because of its simplicity and reliability. For a high-volume pipeline transferring large datasets continuously, shared memory with semaphore-based synchronisation may provide better performance, subject to the measured results.