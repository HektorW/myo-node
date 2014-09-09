{
	"targets": [{
		"target_name": "myo",
		"sources": ["src/myo.cpp"],
		"conditions": [
			["OS=='win'", {
				"include_dirs": [
					"src/lib/myo/include"
				],
				"libraries": [
					"../src/lib/myo/lib/myo64.lib"
				]
			}]
		]
	}]
}