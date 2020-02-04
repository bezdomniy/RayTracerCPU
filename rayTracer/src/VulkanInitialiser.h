// #pragma once

// //#include <vulkan/vulkan.h>
// #include <optional>
// #include <set>
// #define GLFW_INCLUDE_VULKAN
// #include <GLFW/glfw3.h>

// #include <vector>
// #include <iostream>

// class VulkanApp {
// private:
// 	const int WIDTH = 800;
// 	const int HEIGHT = 600;

// #ifdef NDEBUG
// 	const bool enableValidationLayers = false;
// #else
// 	const bool enableValidationLayers = true;
// #endif

// 	GLFWwindow* window;
// 	VkInstance m_instance = VK_NULL_HANDLE;
// 	VkDebugUtilsMessengerEXT debugMessenger;
// 	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
// 	VkQueue graphicsQueue;
// 	VkDevice device = VK_NULL_HANDLE;
// 	VkSurfaceKHR surface;
// 	VkQueue presentQueue;


// 	const std::vector<const char*> validationLayers = {
// 		"VK_LAYER_KHRONOS_validation"
// 	};

// 	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
// 		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

// 		return VK_FALSE;
// 	}

// 	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

// 	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
// 	void setupDebugMessenger();


// public:
// 	VulkanApp();
// 	~VulkanApp();

// 	struct QueueFamilyIndices {
// 		std::optional<uint32_t> graphicsFamily;
// 		std::optional<uint32_t> presentFamily;
// 		bool isComplete() {
// 			return graphicsFamily.has_value() && presentFamily.has_value();
// 		}
// 	};

// 	void initVulkan();
// 	void initWindow();

// 	VkResult createInstance();

// 	bool isDeviceSuitable(VkPhysicalDevice device);
// 	void pickPhysicalDevice();
// 	void createLogicalDevice();
// 	void createSurface();
// 	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

// 	std::vector<const char*> getRequiredExtensions();
// };