use sysinfo::{CpuExt, System, SystemExt};

const NOT_RETRIEVED: &str = "NOT_RETRIEVED";

#[derive(Debug)]
pub struct HardwareInfo {
    serialnumber: String,
    hwtype: String,
    model: String,
}

impl HardwareInfo {
    pub fn new() -> Self {
        HardwareInfo {
            serialnumber: String::new(),
            hwtype: String::new(),
            model: String::new(),
        }
    }

    pub fn serialnumber(&self) -> &str {
        &self.serialnumber
    }

    pub fn hwtype(&self) -> &str {
        &self.hwtype
    }

    pub fn model(&self) -> &str {
        &self.model
    }
}

#[derive(Debug)]
pub struct NetworkInfo {
    hostname: String,
    ip: String,
    macaddress: String,
}

impl NetworkInfo {
    pub fn new() -> Self {
        NetworkInfo {
            hostname: String::new(),
            ip: String::new(),
            macaddress: String::new(),
        }
    }

    pub fn hostname(&self) -> &str {
        &self.hostname
    }

    pub fn ip(&self) -> &str {
        &self.ip
    }

    pub fn macaddress(&self) -> &str {
        &self.macaddress
    }
}

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
    network_info: &mut NetworkInfo,
    cpu_info: &mut CpuInfo,
    memory_info: &mut MemoryInfo,
    os_info: &mut OsInfo,
) {
    let mut sys = System::new_all();
    sys.refresh_all();

    network_info.hostname = sys.host_name().expect(NOT_RETRIEVED);

    cpu_info.model = sys.cpus()[0].name().to_string();
    cpu_info.ncpus = sys.cpus().len();

    memory_info.physmem = sys.total_memory();
    memory_info.swaptotal = sys.total_swap();

    os_info.name = sys.name().expect(NOT_RETRIEVED);
    os_info.release = sys.os_version().expect(NOT_RETRIEVED);
}
