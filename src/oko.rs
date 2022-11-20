use sysinfo::{CpuExt, System, SystemExt};

const NOT_IMPELMENTED: &str = "NOT_IMPLEMENTED";
const NOT_RETRIEVED: &str = "NOT_RETRIEVED";

#[derive(Debug)]
pub struct Device {
    hw_serial_number: String,
    hw_type: String,
    hw_model: String,
    net_host_name: String,
    net_ipv4: String,
    net_mac: String,
    cpu_arch: String,
    cpu_model: String,
    cpu_ncpus: usize,
    mem_phys_total: u64,
    mem_swap_total: u64,
    os_name: String,
    os_release: String,
}

impl Device {
    pub fn new() -> Self {
        Device {
            hw_serial_number: String::new(),
            hw_type: String::new(),
            hw_model: String::new(),
            net_host_name: String::new(),
            net_ipv4: String::new(),
            net_mac: String::new(),
            cpu_arch: String::new(),
            cpu_model: String::new(),
            cpu_ncpus: 0,
            mem_phys_total: 0,
            mem_swap_total: 0,
            os_name: String::new(),
            os_release: String::new(),
        }
    }

    pub fn hw_serial_number(&self) -> &str {
        &self.hw_serial_number
    }

    pub fn hw_type(&self) -> &str {
        &self.hw_type
    }

    pub fn hw_model(&self) -> &str {
        &self.hw_model
    }

    pub fn net_host_name(&self) -> &str {
        &self.net_host_name
    }

    pub fn net_ipv4(&self) -> &str {
        &self.net_ipv4
    }

    pub fn net_mac(&self) -> &str {
        &self.net_mac
    }

    pub fn cpu_arch(&self) -> &str {
        &self.cpu_arch
    }

    pub fn cpu_model(&self) -> &str {
        &self.cpu_model
    }

    pub fn cpu_ncpus(&self) -> usize {
        self.cpu_ncpus
    }

    pub fn mem_phys_total(&self) -> u64 {
        self.mem_phys_total
    }

    pub fn mem_swap_total(&self) -> u64 {
        self.mem_swap_total
    }

    pub fn os_name(&self) -> &str {
        &self.os_name
    }

    pub fn os_release(&self) -> &str {
        &self.os_release
    }
}

pub fn collect_machine_info(device: &mut Device) {
    let mut sys = System::new_all();
    sys.refresh_all();

    device.net_host_name = sys.host_name().expect(NOT_RETRIEVED);

    device.cpu_arch = NOT_IMPELMENTED.to_string();
    device.cpu_model = sys.global_cpu_info().brand().to_string();
    device.cpu_ncpus = sys.cpus().len();

    device.mem_phys_total = sys.total_memory();
    device.mem_swap_total = sys.total_swap();

    device.os_name = sys.name().expect(NOT_RETRIEVED);
    device.os_release = sys.os_version().expect(NOT_RETRIEVED);
}
