create_clock -period 20 [get_ports CLOCK_50]
create_clock -period 100 [get_ports GPIO_0_D5M_PIXCLK]

derive_pll_clocks
derive_clock_uncertainty
