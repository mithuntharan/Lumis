#include <atomic>
#include <cmath>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include "Camera.h"
#include "Material.h"
#include "PixelBuffer.h"
#include "Ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "Vector3.h"
#include "Window.h"

constexpr int IMAGE_WIDTH = 1920;
constexpr int IMAGE_HEIGHT = 1080;
constexpr int SAMPLES_PER_PIXEL = 100;
constexpr int MAX_DEPTH = 100;

static Lumis::Vector3 ray_color(const Lumis::Ray &ray,
                                const Lumis::Scene &scene, const int depth) {
  if (depth <= 0) {
    return {0.0f, 0.0f, 0.0f};
  }

  Lumis::HitRecord rec;
  if (scene.hit(ray, 0.001f, 1e30f, rec)) {
    Lumis::Ray scattered;
    Lumis::Vector3 attenuation;
    if (rec.m_material != nullptr &&
        rec.m_material->scatter(ray, rec, attenuation, scattered)) {
      return attenuation * ray_color(scattered, scene, depth - 1);
    }
    return {0.0f, 0.0f, 0.0f};
  }

  const float t = 0.5f * (ray.direction.normalized().v_y + 1.0f);
  return (1.0f - t) * Lumis::Vector3(1.0f, 1.0f, 1.0f) +
         t * Lumis::Vector3(0.5f, 0.7f, 1.0f);
}

static bool
overlapsAny(const Lumis::Vector3 &candidateCenter, const float candidateRadius,
            const std::vector<std::unique_ptr<Lumis::Sphere>> &spheres) {
  for (const auto &s : spheres) {
    if (s->getRadius() > 50.0f) {
      continue;
    }
    const float dx = candidateCenter.v_x - s->getCenter().v_x;
    const float dz = candidateCenter.v_z - s->getCenter().v_z;
    const float minDist = candidateRadius + s->getRadius() + 0.12f;
    if (dx * dx + dz * dz < minDist * minDist) {
      return true;
    }
  }
  return false;
}

int main() {
  std::vector<std::unique_ptr<Lumis::Material>> materials;
  std::vector<std::unique_ptr<Lumis::Sphere>> spheres;
  Lumis::Scene scene;

  auto addSphere = [&](const Lumis::Vector3 &center, const float radius,
                       std::unique_ptr<Lumis::Material> mat) {
    const Lumis::Material *matPtr = mat.get();
    materials.push_back(std::move(mat));
    auto sphere = std::make_unique<Lumis::Sphere>(center, radius, matPtr);
    scene.add(sphere.get());
    spheres.push_back(std::move(sphere));
  };

  // Ground sphere
  constexpr float groundRadius = 1000.0f;
  addSphere(
      {0.0f, -groundRadius, 0.0f}, groundRadius,
      std::make_unique<Lumis::Lambertian>(Lumis::Vector3(0.45f, 0.45f, 0.42f)));

  // Center hollow glass sphere (outer shell + smaller inner sphere with
  // inverted IOR)
  addSphere({0.0f, 1.0f, 0.0f}, 1.0f,
            std::make_unique<Lumis::Dielectric>(1.5f));
  addSphere({0.0f, 1.0f, 0.0f}, 0.90f,
            std::make_unique<Lumis::Dielectric>(1.0f / 1.5f));

  // Left polished mirror-like metal sphere
  addSphere({-2.2f, 1.0f, -0.3f}, 1.0f,
            std::make_unique<Lumis::Metal>(Lumis::Vector3(0.88f, 0.90f, 0.92f),
                                           0.0f));

  // Right brushed copper-colored metal sphere
  addSphere({2.2f, 1.0f, -0.3f}, 1.0f,
            std::make_unique<Lumis::Metal>(Lumis::Vector3(0.85f, 0.48f, 0.30f),
                                           0.35f));

  // Muted matte sphere behind the glass
  addSphere(
      {0.0f, 0.85f, -2.5f}, 0.85f,
      std::make_unique<Lumis::Lambertian>(Lumis::Vector3(0.45f, 0.52f, 0.42f)));

  // Foreground glossy plastic spheres (red, blue, green)
  addSphere({-1.15f, 0.45f, 1.65f}, 0.45f,
            std::make_unique<Lumis::Plastic>(
                Lumis::Vector3(0.82f, 0.12f, 0.12f), 0.04f, 1.5f));
  addSphere({0.05f, 0.40f, 1.95f}, 0.40f,
            std::make_unique<Lumis::Plastic>(
                Lumis::Vector3(0.12f, 0.30f, 0.85f), 0.04f, 1.5f));
  addSphere({1.20f, 0.45f, 1.60f}, 0.45f,
            std::make_unique<Lumis::Plastic>(
                Lumis::Vector3(0.14f, 0.72f, 0.25f), 0.04f, 1.5f));

  // 20 small randomly placed spheres with a fixed seed for reproducibility
  std::mt19937 sceneRng(42);
  std::uniform_real_distribution<float> distUnit(0.0f, 1.0f);
  auto randRange = [&](const float minVal, const float maxVal) {
    return minVal + (maxVal - minVal) * distUnit(sceneRng);
  };

  int placedSmall = 0;
  int attempts = 0;
  while (placedSmall < 20 && attempts < 400) {
    attempts++;
    const float r = randRange(0.18f, 0.28f);
    const float x = randRange(-4.2f, 4.2f);
    const float z = randRange(-3.8f, 3.2f);
    const float groundY =
        std::sqrt(groundRadius * groundRadius - x * x - z * z) - groundRadius;
    const Lumis::Vector3 center(x, groundY + r, z);

    if (overlapsAny(center, r, spheres)) {
      continue;
    }

    const float choice = distUnit(sceneRng);
    if (choice < 0.35f) {
      const Lumis::Vector3 albedo(randRange(0.2f, 0.8f), randRange(0.2f, 0.8f),
                                  randRange(0.2f, 0.8f));
      addSphere(center, r, std::make_unique<Lumis::Lambertian>(albedo));
    } else if (choice < 0.65f) {
      const Lumis::Vector3 albedo(randRange(0.5f, 0.95f),
                                  randRange(0.5f, 0.95f),
                                  randRange(0.5f, 0.95f));
      addSphere(center, r,
                std::make_unique<Lumis::Metal>(albedo, randRange(0.0f, 0.4f)));
    } else if (choice < 0.88f) {
      const Lumis::Vector3 albedo(randRange(0.15f, 0.9f),
                                  randRange(0.15f, 0.9f),
                                  randRange(0.15f, 0.9f));
      addSphere(center, r,
                std::make_unique<Lumis::Plastic>(albedo, 0.05f, 1.5f));
    } else {
      addSphere(center, r, std::make_unique<Lumis::Dielectric>(1.5f));
    }
    placedSmall++;
  }

  const Lumis::Vector3 camPos(0.4f, 2.2f, 6.0f);
  const Lumis::Vector3 lookAt(0.0f, 0.85f, 0.0f);
  const float aspectRatio =
      static_cast<float>(IMAGE_WIDTH) / static_cast<float>(IMAGE_HEIGHT);
  Lumis::Camera camera(camPos, (lookAt - camPos).normalized(), 28.0f,
                       aspectRatio);

  Lumis::PixelBuffer pixels(IMAGE_WIDTH, IMAGE_HEIGHT);

  unsigned int threadCount = std::thread::hardware_concurrency();
  if (threadCount == 0) {
    threadCount = 4;
  }

  std::atomic<int> nextRow(0);
  std::atomic<int> completedRows(0);
  std::mutex printMutex;

  std::cout << "Rendering " << IMAGE_WIDTH << "x" << IMAGE_HEIGHT << " ("
            << SAMPLES_PER_PIXEL << " spp, max depth " << MAX_DEPTH << ") on "
            << threadCount << " threads...\n";

  std::vector<std::thread> workers;
  workers.reserve(threadCount);
  for (unsigned int t = 0; t < threadCount; t++) {
    workers.emplace_back([&]() {
      while (true) {
        const int j = nextRow.fetch_add(1);
        if (j >= IMAGE_HEIGHT) {
          break;
        }

        for (int i = 0; i < IMAGE_WIDTH; i++) {
          Lumis::Vector3 colorSum(0.0f, 0.0f, 0.0f);
          for (int s = 0; s < SAMPLES_PER_PIXEL; s++) {
            const float u = static_cast<float>(i) + Lumis::randomFloat();
            const float v = static_cast<float>(j) + Lumis::randomFloat();
            const Lumis::Ray ray =
                camera.generateRay(u, v, IMAGE_WIDTH, IMAGE_HEIGHT);
            colorSum = colorSum + ray_color(ray, scene, MAX_DEPTH);
          }

          const Lumis::Vector3 avgColor =
              colorSum * (1.0f / static_cast<float>(SAMPLES_PER_PIXEL));
          pixels.setPixel(i, j, avgColor);
        }

        const int done = completedRows.fetch_add(1) + 1;
        if (done % 10 == 0 || done == IMAGE_HEIGHT) {
          std::lock_guard<std::mutex> lock(printMutex);
          const int percent = (done * 100) / IMAGE_HEIGHT;
          std::cout << "\rProgress: " << percent << "% (" << done << "/"
                    << IMAGE_HEIGHT << " rows)" << std::flush;
        }
      }
    });
  }

  for (std::thread &worker : workers) {
    worker.join();
  }
  std::cout << "\nRender complete!\n";

  Lumis::Window window(IMAGE_WIDTH, IMAGE_HEIGHT, "LUMIS - Ray Tracer");
  while (!window.shouldClose()) {
    window.update(pixels.getData());
    window.pollEvents();
  }
}
