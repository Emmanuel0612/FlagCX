## Environment Configuration

Refer to the environment setup section in [getting_started.md](getting_started.md).

## Installation and Compilation

Refer to [getting_started.md](getting_started.md) for FlagCX compilation and installation.

## Homogeneous Tests Using FlagCX

## Communication API Test

1. Build and Installation

   Refer to the Communication API test build and installation section in [getting_started.md](getting_started.md).

2. Communication API Test

   ```Plain
   mpirun --allow-run-as-root -np 2 ./test_allreduce -b 128K -e 4G -f 2 -p 1
   ```

   **Description**

   -  `test_allreduce` is a performance benchmark for AllReduce operations built on MPI and FlagCX. Each MPI process is bound to a single GPU. The program runs warm-up iterations followed by timed measurements across a user-defined range of message sizes (minimum, maximum, and step).
   -  For every message size, the benchmark reports:
     - Average latency
     - Estimated bandwidth
     - Buffer fragments for correctness verification

   **Example**

   - Running `test_allreduce` with 2 MPI processes on 2 GPUs starts from 128 KiB and doubles the message size each step (128 KiB, 256 KiB, 512 KiB, 1 MiB …) up to 4 GiB. For each size, the benchmark records bandwidth, latency, and correctness results.

3. Correct Performance Test Output

   ![correct_performance_test_output.png](images/correct_performance_test_output.png)


4. Issues Encountered During Execution

   - During execution, you may see an assertion warning when OpenMPI attempts to establish a connection via InfiniBand (openib BTL) but cannot find an available CPC (Connection Protocol). In this case, the IB port is disabled automatically.This warning does not affect the performance test results.

     ![issues_encountered_during_execution.png](images/issues_encountered_during_execution.png)

     **Solution**

     To suppress this warning, disable `openib` and fall back to TCP by adding the following option to your `mpirun` command.

     ```Plain
     --mca btl ^openib
     ```

   - **MPI Error Warning**

     If you encounter an MPI error during execution, there are two possible solutions:

     **Check Local MPI Installation**

     - Verify your local MPI installation path and set the appropriate environment variables.

     **Install MPI**

     - If MPI is not installed or the local installation is not suitable, download and install MPI.

     - Follow the instructions below:

       ```Plain
       wget https://download.open-mpi.org/release/open-mpi/v4.1/openmpi-4.1.6.tar.gz  
       tar -zxf openmpi-4.1.6.tar.gz  
       cd openmpi-4.1.6  
       ##Configure and Build 
       ./configure --prefix=/usr/local/mpi make -j$(nproc) sudo make install
       ```

## Torch API Test

1. Build and Installation

   Refer to [getting_started.md](getting_started.md) for instructions on building and installing the Torch API test.

2. Torch API Test Execution

   - The test case is located in the build/installation directory.

     ```Plain
     cd ./example/example.py
     ```

   - The test script `run.sh` sets environment variables and device IDs according to the current platform. You may need to modify these variables to match your hardware setup.

     ```Plain
     ##run.sh
     #!/bin/bash
     # Check if the debug flag is provided as an argument
     if [ "$1" == "debug" ]; then
         export NCCL_DEBUG=INFO
         export NCCL_DEBUG_SUBSYS=all
         echo "NCCL debug information enabled."
     else
         unset NCCL_DEBUG
         unset NCCL_DEBUG_SUBSYS
         echo "NCCL debug information disabled."
     fi
     
     export FLAGCX_IB_HCA=mlx5
     export FLAGCX_ENABLE_TOPO_DETECT=TRUE
     export FLAGCX_DEBUG=TRUE
     export FLAGCX_DEBUG_SUBSYS=ALL
     export CUDA_VISIBLE_DEVICES=0,1
     # Need to preload customized gloo library specified for FlagCX linkage
     # export LD_PRELOAD=/usr/local/lib/libgloo.so
     # export LD_PRELOAD=/usr/local/nccl/build/lib/libnccl.so
     export TORCH_DISTRIBUTED_DETAIL=DEBUG
     CMD='torchrun --nproc_per_node 2 --nnodes=1 --node_rank=0 --master_addr="localhost" --master_port=8281 example.py'
     
     echo $CMD
     eval $CMD
     ```

      **Explanation**

       `CMD='torchrun --nproc_per_node=2 --nnodes=1 --node_rank=0 --master_addr="localhost" --master_port=8281 example.py'`

     - `--nproc_per_node=2`: Launch 2 processes on the current machine.
     - `--nnodes=1`: Total number of nodes participating in the training. For homogeneous testing, set to 1.
     - `--node_rank=0`: Rank of the current node among all nodes, starting from 0. For homogeneous testing, fixed at 0.
     - `--master_addr="localhost"`: Address of the master node. For homogeneous testing, `localhost` is sufficient; for heterogeneous testing, specify the reachable IP or hostname of the master node, accessible by all nodes.
     - `--master_port=8281`: Port used by the master node to establish the process group. All nodes must use the same port, which must be free.
     - `example.py`: Torch API test script.
     - Refer to [enviroment_variables.md](enviroment_variables.md) for the meaning and usage of `FLAGCX_XXX` environment variables.

3. Sample Screenshot of Correct Performance Test

   ![sample_screenshot_of_correct_performance_test.png](images/sample_screenshot_of_correct_performance_test.png)

## Homogeneous Training with FlagCX + FlagScale

We conduct our experiments by running the LLaMA3-8B model on Nvidia A800 GPUs.

1. Build and Installation

   Refer to the Environment Setup and Build & Installation sections in [getting_started.md](getting_started.md).

2. Data Preparation and Model Configuration

   - **Data Preparation**

     ```
     cd FlagScale
     mkdir data
     ```

     **Description** A small portion of processed data from the Pile dataset (bin and idx files) is provided: pile_wikipedia_demo. Copy it to the FlagScale/data directory.

   - **Model Configuration 1**

     ```
     cd FlagScale/examples/llama3/conf/ 
     vi train.yaml
     ```

     **Description** The directory contains the following files:

     - `train/` — Training scripts and related files

     - `train.yaml` — Configuration file for **homogeneous training**

       The `train.yaml` file contains four main sections: defaults, experiment, action, and hydra. For most cases, you only need to modify defaults and experiment.

       - Modify `defaults`

         ```
         train: XXXX
         ```

          Replace `XXXX` with `8b`.

       - Modify `experiment`

         ```
         exp_dir: ./outputs_llama3_8b
         ```

         This specifies the output directory for distributed training results.

       - Modify `runner` settings under `experiment`

         ​    **hostfile**: Since this is a homogeneous (single-node) test, comment out the `hostfile` line. Only configure it for heterogeneous (multi-node) setups.

         ​    **envs**: Set GPU device IDs using `CUDA_VISIBLE_DEVICES`, for example:

         ```
         CUDA_VISIBLE_DEVICES: 0,1,2,3,4,5,6,7
         ```

     - `train_hetero.yaml` — Configuration file for **heterogeneous training**

   - **Model Configuration 2**

     ```Plain
     # Multiple model configuration files (xxx.yaml) corresponding to different dataset sizes in this directory
     cd FlagScale/examples/llama3/conf/train 
     vi 8b.yaml 
     ```

     - **`8b.yaml`** **Configuration File**

       The `8b.yaml` file contains three main sections: system, model, and data.

       **System Section**

       Add the following line to enable distributed training with FlagCX:

       ```Plain
       distributed_backend: flagcx
       ```

       **Model Section**

       Configure the training parameters.Use `train_samples` and `global_batch_size` to determine the number of steps:

       ```Plain
       step = train_samples / global_batch_size
       ```

         It is recommended to set it as an integer.

       **Data Section**

       Modify the following parameters:

       - **data_path**: Set this to the `cache` directory under the data prepared in the previous step.

       - **tokenizer_path**: Download the tokenizer from the official website corresponding to your model and set the path here.

   - **Tokenizer Download**

     **Description:**

     Download the tokenizer corresponding to the model. The files are available at: [Meta-LLaMA-3-8B-Instruct Tokenizer](https://www.modelscope.cn/models/LLM-Research/Meta-Llama-3-8B-Instruct/files?utm_source=chatgpt.com).

     **Instructions:**

     - It is recommended to download the tokenizer via the command line.

     - Place the downloaded tokenizer files in the path specified by `tokenizer_path` in your configuration (`8b.yaml`).

     **Example:**

     ```Plain
     ## Download the tokenizer to the current directory
     cd FlagScale/examples/llama3
     modelscope download --model LLM-Research/Meta-Llama-3-8B-Instruct [XXXX] --local_dir ./
     ```

     **Description**

     `[XXXX]` refers to the tokenizer files corresponding to Meta-LLaMA-3-8B-Instruct, for example:

     - `tokenizer.json`
     - `tokenizer_config.json`
     - `config.json`
     - `configuration.json`
     - `generation_config.json`

     These files should be placed in the directory specified by `tokenizer_path` in your configuration (`8b.yaml`).

4. Distributed Training

   ```Plain
   cd FlagScale
   ##Start Distributed Training
   python run.py --config-path ./examples/llama3/conf --config-name train action=run 
   ## Stop Distributed Training
   python run.py --config-path ./examples/llama3/conf --config-name train action=stop 
   ```

   After starting distributed training, the configuration information will be printed, and a run script will be generated at:

   ```Plain
   flagscale/outputs_llama3_8b/logs/scripts/host_0_localhost_run.sh
   ```

   The training output files can be found in:

   ```Plain
   flagscale/outputs_llama3_8b
   ```

   **Notes:**

   - You can inspect the run script to verify the commands and environment settings used for the training.

   - All logs and model checkpoints will be saved under the output directory.

     ![distributed_training.png](images/distributed_training.png)

## Heterogeneous Tests Using FlagCX

## Communication API Test

1. Build and Installation

   Refer to the Environment Setup, Creating Symbolic Links, and Build & Installation sections in [getting_started.md](getting_started.md).

2. Verify MPICH Installation

   ```Plain
   # Check if MPICH is installed
   cd /workspace/mpich-4.2.3
   ```

3. Makefile and Environment Variable Configuration

   ```
   # Navigate to the Communication API test directory
   cd /root/FlagCX/test/perf 
   
   # Open the Makefile
   vi Makefile
       # Modify the MPI path to match the one used in step 2
       MPI_HOME ?= /workspace/mpich-4.2.3/build/ 
   :q # Save and exit
   
   # Configure environment variables
   export LD_LIBRARY_PATH=/workspace/mpich-4.2.3/build/lib:$LD_LIBRARY_PATH
   ```

4. Heterogeneous Communication API Test

   - Ensure that Host 1, Host 2, … are all configured as described above and can correctly run the homogeneous Communication API test on their respective platforms.


   - Verify that the ports on Host 1, Host 2, … are `<xxx>` and keep them consistent across all hosts.


   - Before running the heterogeneous Communication API test script on Host 1, configure the port number environment variable:

     ```Plain
     export HYDRA_LAUNCHER_EXTRA_ARGS="-p 8010"
     ```
     
     Here, `8010` should match the configuration set during SSH passwordless login.

   - Run the heterogeneous Communication API test script on Host 1:

     ```Plain
     ./run.sh
     ```

     ```Plain
     /workspace/mpich-4.2.3/build/bin/mpirun \
       -np 2 -hosts 10.1.15.233:1,10.1.15.67:1 \
       -env PATH=/workspace/mpich-4.2.3/build/bin \
       -env LD_LIBRARY_PATH=/workspace/mpich-4.2.3/build/lib:/root/FlagCX/build/lib:/usr/local/mpi/lib/:/opt/maca/ompi/lib \
       -env FLAGCX_IB_HCA=mlx5 \
       -env FLAGCX_ENABLE_TOPO_DETECT=TRUE \
       -env FLAGCX_DEBUG=INFO \
       -env FLAGCX_DEBUG_SUBSYS=INIT \
       /root/FlagCX/test/perf/test_allreduce -b 128K -e 4G -f 2 -w 5 -n 100 -p 1`
     ```

     - Refer to [enviroment_variables.md](enviroment_variables.md) for the meaning and usage of `FLAGCX_XXX` environment variables.


   - **Note:** When using two GPUs per node in the heterogeneous Communication API test, some warnings may indicate that each node only has 1 GPU active. In this case, FlagCX will skip GPU-to-GPU AllReduce and fall back to host-based communication.

     - As a result, GPU utilization may show 0%, and the overall AllReduce runtime may be much longer.
     
     - However, the computation results are correct, and this behavior is expected.

     - To fully utilize GPU acceleration for heterogeneous testing, use 2+2 GPUs (4 GPUs total) across the nodes.
     
       ![heterogeneous_communication_api_test.png](images/heterogeneous_communication_api_test.png)