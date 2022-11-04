use sysinfo::{System, SystemExt};

const NOT_RETRIEVED: &str = "NOT_RETRIEVED";

#[derive(Debug)]
pub struct CpuInfo {
    arch: String,
    model: String,
    ncpus: usize,
}

impl CpuInfo {
    pub fn new() -> Self {
        CpuInfo {
            arch: String::new(),
            model: String::new(),
            ncpus: 0,
        }
    }

    pub fn arch(&self) -> &str {
        &self.arch
    }

    pub fn model(&self) -> &str {
        &self.model
    }

    pub fn ncpus(&self) -> usize {
        self.ncpus
    }
}

#[derive(Debug)]
pub struct MemoryInfo {
    physmem: u64,
    swaptotal: u64,
}

impl MemoryInfo {
    pub fn new() -> Self {
        MemoryInfo {
            physmem: 0,
            swaptotal: 0,
        }
    }

    pub fn physmem(&self) -> u64 {
        self.physmem
    }

    pub fn swaptotal(&self) -> u64 {
        self.swaptotal
    }
}

#[derive(Debug)]
pub struct OsInfo {
    name: String,
    release: String,
}

impl OsInfo {
    pub fn new() -> Self {
        OsInfo {
            name: String::new(),
            release: String::new(),
        }
    }

    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn release(&self) -> &str {
        &self.release
    }
}

pub fn collect_machine_info(
    cpu_info: &mut CpuInfo,
    memory_info: &mut MemoryInfo,
    os_info: &mut OsInfo,
) {
    let mut sys = System::new_all();
    sys.refresh_all();

    cpu_info.ncpus = sys.cpus().len();

    memory_info.physmem = sys.total_memory();
    memory_info.swaptotal = sys.total_swap();

    os_info.name = sys.name().expect(NOT_RETRIEVED);
    os_info.release = sys.os_version().expect(NOT_RETRIEVED);
}
