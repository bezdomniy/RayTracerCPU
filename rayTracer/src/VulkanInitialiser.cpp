// #include "VulkanInitialiser.h"

// VulkanApp::VulkanApp()
// {
// }

// VulkanApp::~VulkanApp() {}

// void VulkanApp::initVulkan() {
// 	createInstance();
// 	setupDebugMessenger();
// 	createSurface();
// 	pickPhysicalDevice();
// 	createLogicalDevice();
// }

// void VulkanApp::initWindow() {
// 	glfwInit();

// 	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
// 	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

// 	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Ray Tracer", nullptr, nullptr);
// }

// VkResult VulkanApp::createInstance()
// {
// 	VkResult result = VK_SUCCESS;
// 	VkApplicationInfo appInfo = {};
// 	VkInstanceCreateInfo instanceCreateInfo = {};

// 	// A generic application info structure
// 	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
// 	appInfo.pApplicationName = "RayTracer";
// 	appInfo.applicationVersion = 1;
// 	appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

// 	auto extensions = getRequiredExtensions();

// 	// Create the instance.
// 	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
// 	instanceCreateInfo.pApplicationInfo = &appInfo;
// 	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
// 	instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

// 	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
// 	if (enableValidationLayers) {
// 		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
// 		instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();

// 		populateDebugMessengerCreateInfo(debugCreateInfo);
// 		instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
// 	}
// 	else {
// 		instanceCreateInfo.enabledLayerCount = 0;
// 		instanceCreateInfo.pNext = nullptr;
// 	}
// 	instanceCreateInfo.enabledLayerCount = 0;
// 	result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);

// 	if (result == VK_SUCCESS) {
// 		// Load available extensions
// 		uint32_t extensionCount = 0;
// 		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
// 		std::vector<VkExtensionProperties> extensions(extensionCount);
// 		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

// 		for (auto& item : extensions) {
// 			std::cout << item.extensionName << std::endl;
// 		}
// 	}

// 	if (result != VK_SUCCESS) throw std::runtime_error("failed to create instance!");

// 	return result;

// }

// void VulkanApp::setupDebugMessenger() {
// 	if (!enableValidationLayers) return;

// 	VkDebugUtilsMessengerCreateInfoEXT createInfo;
// 	populateDebugMessengerCreateInfo(createInfo);

// 	if (CreateDebugUtilsMessengerEXT(this->m_instance, &createInfo, nullptr, &this->debugMessenger) != VK_SUCCESS) {
// 		throw std::runtime_error("failed to set up debug messenger!");
// 	}
// }

// VkResult VulkanApp::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
// 	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
// 	if (func != nullptr) {
// 		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
// 	}
// 	else {
// 		return VK_ERROR_EXTENSION_NOT_PRESENT;
// 	}
// }

// void VulkanApp::pickPhysicalDevice() {
// 	uint32_t deviceCount = 0;
// 	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

// 	if (deviceCount == 0) {
// 		throw std::runtime_error("failed to find GPUs with Vulkan support!");
// 	}

// 	std::vector<VkPhysicalDevice> devices(deviceCount);
// 	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

// 	for (const auto& device : devices) {
// 		if (isDeviceSuitable(device)) {
// 			m_physicalDevice = device;
// 			break;
// 		}
// 	}

// 	if (m_physicalDevice == VK_NULL_HANDLE) {
// 		throw std::runtime_error("failed to find a suitable GPU!");
// 	}
// }

// bool VulkanApp::isDeviceSuitable(VkPhysicalDevice device)
// {
// 	QueueFamilyIndices indices = findQueueFamilies(device);

// 	return indices.isComplete();
// }

// VulkanApp::QueueFamilyIndices VulkanApp::findQueueFamilies(VkPhysicalDevice device) {
// 	QueueFamilyIndices indices;

// 	uint32_t queueFamilyCount = 0;
// 	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

// 	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
// 	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

// 	int i = 0;
// 	for (const auto& queueFamily : queueFamilies) {
// 		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
// 			indices.graphicsFamily = i;
// 		}

// 		VkBool32 presentSupport = false;
// 		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, this->surface, &presentSupport);

// 		if (presentSupport) {
// 			indices.presentFamily = i;
// 		}

// 		if (indices.isComplete()) {
// 			break;
// 		}

// 		i++;
// 	}



// 	return indices;
// }

// std::vector<const char*> VulkanApp::getRequiredExtensions() {
// 	uint32_t glfwExtensionCount = 0;
// 	const char** glfwExtensions;
// 	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

// 	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

// 	if (enableValidationLayers) {
// 		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
// 	}

// 	return extensions;
// }

// void VulkanApp::createLogicalDevice() {
// 	QueueFamilyIndices indices = findQueueFamilies(this->m_physicalDevice);

// 	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
// 	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

// 	float queuePriority = 1.0f;
// 	for (uint32_t queueFamily : uniqueQueueFamilies) {
// 		VkDeviceQueueCreateInfo queueCreateInfo = {};
// 		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
// 		queueCreateInfo.queueFamilyIndex = queueFamily;
// 		queueCreateInfo.queueCount = 1;
// 		queueCreateInfo.pQueuePriorities = &queuePriority;
// 		queueCreateInfos.push_back(queueCreateInfo);
// 	}

// 	VkPhysicalDeviceFeatures deviceFeatures = {};

// 	VkDeviceCreateInfo createInfo = {};
// 	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

// 	createInfo.pQueueCreateInfos = queueCreateInfos.data();
// 	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;

// 	createInfo.pEnabledFeatures = &deviceFeatures;

// 	createInfo.enabledExtensionCount = 0;

// 	if (enableValidationLayers) {
// 		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
// 		createInfo.ppEnabledLayerNames = validationLayers.data();
// 	}
// 	else {
// 		createInfo.enabledLayerCount = 0;
// 	}

// 	if (vkCreateDevice(this->m_physicalDevice, &createInfo, nullptr, &this->device) != VK_SUCCESS) {
// 		throw std::runtime_error("failed to create logical device!");
// 	}

// 	vkGetDeviceQueue(this->device, indices.graphicsFamily.value(), 0, &this->graphicsQueue);
// }

// void VulkanApp::createSurface() {
// 	if (glfwCreateWindowSurface(this->m_instance, this->window, nullptr, &this->surface) != VK_SUCCESS) {
// 		throw std::runtime_error("failed to create window surface!");
// 	}
// }

// void VulkanApp::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
// 	createInfo = {};
// 	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
// 	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
// 	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
// 	createInfo.pfnUserCallback = debugCallback;
// }